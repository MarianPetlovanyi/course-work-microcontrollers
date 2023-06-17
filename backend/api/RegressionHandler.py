from flask_restful import Api, Resource, reqparse
from sqlalchemy import create_engine,text
import pandas as pd
import matplotlib.pyplot as plt
import os
import numpy as np

def generate_photo(df, points_df, output_file):
    x = np.linspace(points_df["x"].min()-2, points_df["x"].max()+2, 100)  # Range of x values
    y = np.zeros_like(x)  # Initialize y as an array of zeros
    df =df.drop("id", axis=1)
    
    params = np.expand_dims(df, axis=-1)
    for n in range(len(params[0])):
      y += params[0,n]*(x**n)

    plt.scatter(points_df['x'], points_df['y'], color='blue')
    plt.plot(x, y, color='red', label='y = f(x)')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('Scatter Plot with Function')
    plt.legend()
    plt.savefig(output_file, dpi=300)
    plt.close()

class RegressionHandler(Resource):
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
    print(query)
    df = pd.read_sql_query(query, engine)
    
    points_query = "SELECT * from points_table_{};".format(param[17:-2])
    print(points_query)
    points_df =pd.read_sql_query(points_query, engine) 
    print(points_df)
    print(df)   
    image_path = "/home/mpetlyovanyi/course_work/python-backend/frontend/src/scatter_plot_{}.png".format(param)
    if not os.path.exists(image_path):
       generate_photo(df, points_df, image_path)
    
    result = df.to_json(orient='records')
    return result
