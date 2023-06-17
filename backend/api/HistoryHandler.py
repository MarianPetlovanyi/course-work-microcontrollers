from flask_restful import Api, Resource, reqparse
from sqlalchemy import create_engine,text

class HistoryHandler(Resource):
  def get(self):
    your_username = "vagrant"
    your_password = "vagrant"
    your_host = "192.168.33.50"
    your_database = "example_db"
    engine = create_engine( 'mysql+mysqlconnector://{your_username}:{your_password}@{your_host}/{your_database}'
            .format(your_username = your_username,your_password = your_password, 
                your_host =your_host,your_database =your_database))
    conn = engine.connect()
    query = "SELECT table_name FROM information_schema.tables WHERE table_schema = 'example_db';"
    result = conn.execute(text(query)).fetchall()
    result_string = ','.join([str(row[0]) for row in result])
    return {
      'resultStatus': 'SUCCESS',
      'message': result_string
      }

  def post(self):
    print(self)
    parser = reqparse.RequestParser()
    parser.add_argument('type', type=str)
    parser.add_argument('message', type=str)

    args = parser.parse_args()

    print(args)
    # note, the post req from frontend needs to match the strings here (e.g. 'type and 'message')

    request_type = args['type']
    request_json = args['message']
    # ret_status, ret_msg = ReturnData(request_type, request_json)
    # currently just returning the req straight
    ret_status = request_type
    ret_msg = request_json

    if ret_msg:
      message = "Your Message Requested: {}".format(ret_msg)
    else:
      message = "No Msg"
    
    final_ret = {"status": "Success", "message": message}

    return final_ret
