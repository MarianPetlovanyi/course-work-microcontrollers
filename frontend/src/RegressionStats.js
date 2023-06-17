import React, { useEffect, useState } from 'react';
import { useParams } from 'react-router-dom';
import axios from 'axios';
import JsonDataDisplay from './JsonDataDisplay.js';

const RegressionStats = () => {
  const { date } = useParams();
  const [data, setData] = useState('');
  const [errorMessage, setErrorMessage] = useState('');
  const [item, setItem] = useState(null);
  const [imageLoaded, setImageLoaded] = useState(false);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    const timer = setTimeout(() => {
      setIsLoading(false);
    }, 1000); // Delay in milliseconds (0.5 seconds)

    return () => clearTimeout(timer); // Clean up the timer on unmount
  }, []);
  
  
  useEffect(() => {
    const fetchData = async () => {
      try {
        const response = await axios.get(`http://localhost:5000/regression/${date}`);
        setData(response.data);
      } catch (error) {
        console.log('Error:', error);
        setErrorMessage('Error fetching data');
      }
    };

    fetchData();
  }, [date]);

  useEffect(() => {
    if (data) {
      try {
        setItem(JSON.parse(data));
      } catch (error) {
        console.log('Error parsing JSON:', error);
        setErrorMessage('Error parsing JSON data');
      }
    }
  }, [data]);
  
  useEffect(() => {
    const loadImage = () => {
      const img = new Image();
      img.onload = () => setImageLoaded(true);
      img.onerror = () => {
        console.log('Error loading image');
        setImageLoaded(true);
      };
      img.src = `./scatter_plot_${date}.png`;
    };

    loadImage();
  }, [date]);

  return (
    <div>
      {isLoading ? (
        // Render a loading indicator or placeholder content
        <div style={{ display: 'flex', justifyContent: 'center', alignItems: 'center', height: '100vh' }}>
         <div>Loading...</div>
        </div>
      ) : (
        <div>
      <div style={{ display: 'flex'}}>
      <h1>Regression</h1>
    </div>
      {errorMessage ? (
        <p>{errorMessage}</p>
      ) : (
        item && <JsonDataDisplay jsonData={item} />
      )}
      {imageLoaded ? (
        <div>
          <img src={require(`./scatter_plot_${date}.png`)} alt="" width="50%" style={{ position: 'absolute', top: '80px', right: '16px' }} />
        </div>
      ) : (
        <p>Loading image...</p>
      )}
    </div>
      )}
    </div>
  );
  
  
  
};

export default RegressionStats;

