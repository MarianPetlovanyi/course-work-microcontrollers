import React, { useEffect, useState } from 'react';
import { BrowserRouter, Routes, Route, NavLink } from 'react-router-dom';
import axios from 'axios';
import TableComponent from './TableComponent';
import PointsStats from './PointsStats';
import RegressionStats from './RegressionStats';
import Home from './Home';
function App() {

  return (
    <>
      <BrowserRouter>
        <div
          style={{
            display: 'flex',
            background: 'black',
            padding: '5px 0 5px 5px',
            fontSize: '20px',
          }}
        >
          <div style={{ margin: '10px' }}>
            <NavLink
              to="/"
              style={{color:  'white'}}
            >
              Home
            </NavLink>
          </div>
        </div>
        <Routes>
          <Route exact path="/" element={<Home />} />
          <Route
            path="/points/:date"
            element={<PointsStats />}
          />
          <Route
            path="/regression/:date"
            element={<RegressionStats />}
          />
        </Routes>
      </BrowserRouter>
    </>
  );
}

export default App;

