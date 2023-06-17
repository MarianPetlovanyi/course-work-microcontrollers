from flask import Flask, send_from_directory, make_response, g
from flask_restful import Api, Resource, reqparse
from flask_cors import CORS
from api.HistoryHandler import HistoryHandler
from api.PointsHandler import PointsHandler
from api.RegressionHandler import RegressionHandler
from sqlalchemy import create_engine
import pandas as pd
import matplotlib.pyplot as plt

app = Flask(__name__)
CORS(app)  # Comment this on deployment
api = Api(app)


def generate_photo(df, output_file):
    plt.scatter(df['x'], df['y'])
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title('Scatter Plot')
    plt.savefig(output_file, dpi=300)
    plt.close()

@app.route("/", defaults={'path': ''})
def serve(path):
    return send_from_directory(app.static_folder, 'index.html')

    
api.add_resource(PointsHandler, '/points/<string:param>')
api.add_resource(RegressionHandler, '/regression/<string:param>')
api.add_resource(HistoryHandler, '/flask/history')

