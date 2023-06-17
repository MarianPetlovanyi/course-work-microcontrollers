import TableComponent from './TableComponent';
import axios from 'axios';
import React, { useEffect, useState } from 'react';

async function fetchData() {
  try {
    const response = await axios.get('http://localhost:5000/flask/history');
    const data = response.data;
    return data.message; 
  } catch (error) {
    console.log('Error:', error);
    return null;
  }
}

const Home = () => {
  const [tables, setTables] = useState([]);
 
  useEffect(() => {

    if (tables.length === 0) {
      fetchData()
        .then((data) => {
          if (data) {
            const tableNames = data.split(',');
            setTables(tableNames);
          }
        })
        .catch((error) => {
          console.log('Error:', error);
        });
    }
  }, [tables]);

  return (
    <>
      <TableComponent tables={tables} />
    </>
  );
};

export default Home;

