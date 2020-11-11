#include <iostream>
#include <windows.h>
#include <fstream>
#include <future>
#include <string>
#include <memory.h>
#include "PE.h"
#include <bitset>
#include <sstream>
#include <string.h>

using namespace std;

constexpr std::size_t
align_up(std::size_t value, std::size_t alignment) noexcept
{
    return (value + alignment - 1) & ~(alignment - 1);
}
namespace PE
{
    void PE_FILE::set_sizes(size_t size_ids_, size_t size_dos_stub_,
                            size_t size_inh32_, size_t size_ish_, size_t size_sections_)
    {
        this->size_ids = size_ids_;
        this->size_dos_stub = size_dos_stub_;
        this->size_inh32 = size_inh32_;
        this->size_ish = size_ish_ + sizeof(IMAGE_SECTION_HEADER);
        this->size_sections = size_sections_;
    }
    tuple<bool, char *, streampos> OpenBinary(string filename)
    {
        auto flag = false;
        fstream::pos_type size{};
        char *bin{};
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
    PE_FILE ParsePE(const char *PE)
    {
        PE_FILE pefile{};
        memcpy_s(&pefile.ids, sizeof(IMAGE_DOS_HEADER), PE, sizeof(IMAGE_DOS_HEADER));
        memcpy_s(&pefile.inh32, sizeof(IMAGE_NT_HEADERS32), PE + pefile.ids.e_lfanew,
                 sizeof(IMAGE_NT_HEADERS32));                // address of PE header = e_lfanew
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
        auto First_Section_Header = PE_Header + 0x18 +
                                    pefile.inh32.FileHeader.SizeOfOptionalHeader;
        // First Section: PE_header + sizeof FileHeader + sizeof Optional Header
        // copy section headers
        for (auto i = 0; i < pefile.inh32.FileHeader.NumberOfSections; ++i)
        {
            memcpy_s(&pefile.ish[i], sizeof(IMAGE_SECTION_HEADER),
                     First_Section_Header + (i * sizeof(IMAGE_SECTION_HEADER)),
                     sizeof(IMAGE_SECTION_HEADER));
        }
        for (auto i = 0; i < pefile.inh32.FileHeader.NumberOfSections; ++i)
        {
            shared_ptr<char> t_char(new char[pefile.ish[i].SizeOfRawData]{},
                                    std::default_delete<char[]>()); // Section
            memcpy_s(t_char.get(), pefile.ish[i].SizeOfRawData,
                     PE + pefile.ish[i].PointerToRawData, pefile.ish[i].SizeOfRawData);
            pefile.Sections.push_back(t_char);
        }
        size_t sections_size{};
        for (WORD i = 0; i < pefile.inh32.FileHeader.NumberOfSections; ++i)
        {
            sections_size += pefile.ish[i].SizeOfRawData;
        }
        pefile.set_sizes(sizeof(pefile.ids), stub_size, sizeof(pefile.inh32),
                         number_of_sections * sizeof(IMAGE_SECTION_HEADER), sections_size);
        return pefile;
    }
    void WriteBinary(PE_FILE pefile, std::string file_name, size_t size)
    {
        pefile.inh32.OptionalHeader.CheckSum = 0;
        auto r_ch = new char[size]{};
        memcpy_s(r_ch, pefile.size_ids, &pefile.ids, pefile.size_ids);
        // First we copy the DOS Header
        memcpy_s(r_ch + pefile.size_ids, pefile.size_dos_stub, pefile.MS_DOS_STUB.data(),
                 pefile.size_dos_stub); // Then the DOS Stub
        memcpy_s(r_ch + pefile.size_ids + pefile.size_dos_stub, pefile.size_inh32, &pefile.inh32,
                 pefile.size_inh32); // Later the 32 bit Headers (optional and mandatory)
        for (auto i = 0; i < pefile.inh32.FileHeader.NumberOfSections + 1; ++i)
        {
            // Copy all sections
            memcpy_s(r_ch + pefile.size_ids + pefile.size_dos_stub + pefile.size_inh32 +
                         i * sizeof(IMAGE_SECTION_HEADER),
                     sizeof(IMAGE_SECTION_HEADER),
                     &pefile.ish[i], sizeof(IMAGE_SECTION_HEADER));
        }
        for (auto i = 0; i < pefile.inh32.FileHeader.NumberOfSections; ++i)
        {
            // Update the maximum size of the data for all sections
            memcpy_s(&r_ch[pefile.ish[i].PointerToRawData], pefile.ish[i].SizeOfRawData,
                     pefile.Sections[i].get(), pefile.ish[i].SizeOfRawData);
        }
        ofstream ofile(file_name, ios::binary | ios::out);
        ofile.write(r_ch, size);
        std::cout << "\nEOF\n"
                  << endl;
        ofile.close();
    }
} // namespace PE