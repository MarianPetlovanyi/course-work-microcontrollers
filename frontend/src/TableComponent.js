import React from 'react';
import { NavLink } from 'react-router-dom';
import { format } from 'react-string-format';
import './TableComponent.css'
const TableComponent = ({ tables }) => {
  const pointsTables = tables.filter((table) => table.startsWith('points_table_'));
  const regressionTables = tables.filter((table) => table.startsWith('regression_table_'));

  return (
    <div>
      <h2>Points Tables</h2>
      <table>
        <thead>
          <tr>
            <th>Table Names</th>
          </tr>
        </thead>
        <tbody>
          {pointsTables.map((table, index) => (
            <tr key={index}>
              <td>
                <NavLink to={format('points/{0}', table)}>{table}</NavLink>
              </td>
            </tr>
          ))}
        </tbody>
      </table>

      <h2>Regression Tables</h2>
      <table style={{  }}>
        <thead>
          <tr>
            <th>Table Names</th>
          </tr>
        </thead>
        <tbody>
          {regressionTables.map((table, index) => (
            <tr key={index}>
              <td>
                <NavLink to={format('regression/{0}', table)}>{table}</NavLink>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default TableComponent;

