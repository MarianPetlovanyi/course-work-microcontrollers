import React from 'react';
import './JsonDataDisplay.css'
const JsonDataDisplay = ({ jsonData }) => {
  if (jsonData.length === 0) {
    return <p>No data available.</p>;
  }

  const columns = Object.keys(jsonData[0]);

  return (
    <table>
      <thead>
        <tr>
          {columns.map((column) => (
            <th key={column}>{column}</th>
          ))}
        </tr>
      </thead>
      <tbody>
        {jsonData.map((item, index) => (
          <tr key={index}>
            {columns.map((column) => (
              <td key={column}>{item[column]}</td>
            ))}
          </tr>
        ))}
      </tbody>
    </table>
  );
};

export default JsonDataDisplay;

