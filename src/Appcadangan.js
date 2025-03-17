import logo from './logo.svg';
import './App.css';
import React, { useEffect, useState } from 'react';

function App() {
  const [data, setData] = useState([]);

  useEffect(() => {
    fetch('https://myiot-5a297-default-rtdb.asia-southeast1.firebasedatabase.app/.json?orderBy=%22$key%22&startAt=%22master%22&endAt=%22master\\uf8ff%22')
      .then(response => response.json())
      .then(data => {
        const tableData = Object.keys(data).map(key => ({
          id: key,
          ...data[key]
        }));
        setData(tableData);
      });
  }, []);

  return (
    <div style={{ backgroundColor: '#f0f0f5', padding: '20px', minHeight: '100vh' }}>
      <h1 style={{ textAlign: 'center', marginBottom: '20px' }}>COOL GUARD</h1>
      {data.map(item => (
  <div
    key={item.id}
    style={{
      border: '1px solid #ddd',
      padding: '10px',
      margin: '10px 0',
      borderRadius: '10px',
      boxShadow: '0 2px 5px rgba(0,0,0,0.1)',
      backgroundColor: '#fff',
    }}
  >
    {data.map(item => (
  <div
    key={item.id}
    style={{
      border: '1px solid #ddd',
      padding: '10px',
      margin: '10px 0',
      borderRadius: '10px',
      boxShadow: '0 2px 5px rgba(0,0,0,0.1)',
      backgroundColor: '#fff',
    }}
  >
    {Object.keys(item).map(key => (
      key !== 'id' && (
        <div
          key={key}
          style={{ display: 'flex', alignItems: 'center', marginBottom: '10px' }}
        >
          <div style={{ flex: '1', fontWeight: 'bold' }}>{key}</div>
          <div style={{ flex: '2' }}>
            {key === 'reading' ? (
              <div
                style={{
                  backgroundColor: item[key] === 0 ? 'blue' : 'red', // Kondisi untuk latar belakang
                  padding: '5px',
                  borderRadius: '5px',
                  color: 'white',
                }}
              >
                {item[key]}
              </div>
            ) : (
              (key === 'img' || key === 'photo') ? (
                <img
                  id={key}
                  src={item[key]}
                  alt={key}
                  style={{
                    width: '100px',
                    height: '100px',
                    borderRadius: key === 'img' ? '50%' : '0',
                    objectFit: 'cover',
                  }}
                />
              ) : key === 'timestamp' ? (
                <TimeDifferenceMenu timestamp={item[key]} />  
              )                 : (

                item[key]
              )
            )}
          </div>
        </div>
      )
    ))}
  </div>
))}
  </div>
))}
    </div>
  );
}

function TimeDifferenceMenu({ timestamp }) {
  const currentTime = Math.floor(Date.now() / 1000); // Waktu saat ini dalam detik
  const timeDiff = currentTime - timestamp;

  const formatTimeDiff = (diff) => {
    if (diff < 60) {
      return `${diff} detik yang lalu`;
    } else if (diff < 3600) {
      const minutes = Math.floor(diff / 60);
      return `${minutes} menit yang lalu`;
    } else if (diff < 86400) {
      const hours = Math.floor(diff / 3600);
      return `${hours} jam yang lalu`;
    } else {
      const days = Math.floor(diff / 86400);
      return `${days} hari yang lalu`;
    }
  };

  return <div>{formatTimeDiff(timeDiff)}</div>;
}
export default App;

