import pandas as pd
import numpy as np
import argparse
from sqlalchemy import create_engine,text

parser = argparse.ArgumentParser()
parser.add_argument('table_name', type=str, help='Table name')
parser.add_argument('k', type=int, help='k dim')
args = parser.parse_args()
table_time=args.table_name[13:]


def show_equation(parameters):
    equation = "y = "
    reverse_parameters = parameters[::-1]
    power = len(reverse_parameters) - 1

    for i, param in enumerate(reverse_parameters):
        if power==0:
            equation += "{} ".format(param)
        elif power==1:
            equation += "{}x ".format(param)
        else:
            equation += "{}x^{} ".format(param, power)
        
        if i < len(reverse_parameters) - 1:
            equation += "+ "
        power -= 1

    print(equation)



def least_squares(df, k):
    X = np.array(df["x"])
    #print(X, np.sum(X))
    y = np.array(df["y"])
    n = len(df.index)
    m = []
    M = []
    

    My = []
    for i in range(k+1):
        My.append(np.sum(y*(X**i)))
    
   # print(My)
    for i in range(k+1):
        for j in range(k+1):
            if(i==0 and j==0):
                m.append(n)
            else:
                m.append(np.sum(X**(j+i)))
        M.append(m)
        m = []

    M = np.array(M)
    #print(M)
    detM = np.linalg.det(M)
    detsM = []
    for i in range(k+1):
        tempM = np.copy(M)
        tempM[:,i] = My
        #print(tempM)
        tempDet = np.linalg.det(tempM)
        detsM.append(tempDet)

    #print(detM)
    #print(detsM)
    parameters = detsM/detM
    #print(parameters)
    show_equation(parameters)

    return parameters


def send_to_database(parameters, k):
    conn = engine.connect()
    query = "CREATE TABLE IF NOT EXISTS regression_table_{}_{} (id INT PRIMARY KEY AUTO_INCREMENT,".format(table_time, k)
    for i in range(len(parameters)):
        if i==len(parameters)-1:
            query = query + "x{} float)".format(i)
        else:
            query = query + "x{} float,".format(i)
    


    query=text(query)
    conn.execute(query)

    insert_query = "INSERT INTO regression_table_{}_{} (".format(table_time, k)
    print(insert_query)
    for i in range(len(parameters)):
        if i==len(parameters)-1:
            insert_query = insert_query + "x{} ) ".format(i)
        else:
            insert_query = insert_query + "x{}, ".format(i)
    
    insert_query = insert_query + "VALUES ("
    for i in range(len(parameters)):
        if i==len(parameters)-1:
            insert_query = insert_query + "{})".format(parameters[i])
        else:
            insert_query = insert_query + "{}, ".format(parameters[i])
    

    insert_query=text(insert_query)
    result = conn.execute(insert_query)
    conn.commit()
    conn.close()


your_username = "vagrant"
your_password = "vagrant"
your_host = "192.168.33.50"
your_database = "example_db"
engine = create_engine(
        'mysql+mysqlconnector://{your_username}:{your_password}@{your_host}/{your_database}'
        .format(your_username = your_username,your_password = your_password,
            your_host =your_host,your_database =your_database))

query = "SELECT * from example_db.{}".format(args.table_name);
df = pd.read_sql_query(query, engine)

res = least_squares(df, args.k)

send_to_database(res, args.k)
