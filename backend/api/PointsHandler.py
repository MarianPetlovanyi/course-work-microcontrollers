from flask_restful import Api, Resource, reqparse
from sqlalchemy import create_engine,text
import pandas as pd
import matplotlib.pyplot as plt
import os

def generate_photo(df, output_file):
    plt.scatter(df['x'], df['y'], color='blue')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('Scatter Plot')
    plt.savefig(output_file, dpi=300)
    plt.close()

class PointsHandler(Resource):
  def get(self, param):
    your_username = "vagrant"
    your_password = "vagrant"
    your_host = "192.168.33.50"
    your_database = "example_db"
    engine = create_engine( 'mysql+mysqlconnector://{your_username}:{your_password}@{your_host}/{your_database}'
            .format(your_username = your_username,your_password = your_password,
                your_host =your_host,your_database =your_database))
    conn = engine.connect()
    query = "SELECT * from {};".format(param)
    df = pd.read_sql_query(query, engine)
    image_path = "/home/mpetlyovanyi/course_work/python-backend/frontend/src/scatter_plot_{}.png".format(param)
    if not os.path.exists(image_path):
       generate_photo(df, image_path)
       print("Image created")
    else:
       print("Image exist")
    result = df.to_json(orient='records')
    return result
