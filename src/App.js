import React, { useEffect, useState } from 'react';
import './App.css';
import config from './config';

const firebaseURL = config.firebaseURL;


function App() {
  const [data, setData] = useState([]);
  const [currentPage, setCurrentPage] = useState('main'); // State untuk mengelola halaman aktif

  useEffect(() => {
    const fetchData = () => {
      fetch(firebaseURL)
        .then(response => response.json())
        .then(data => {
          const tableData = Object.keys(data).map(key => ({
            id: key,
            ...data[key]
          }));
          setData(tableData);
        })
        .catch(error => {
          console.error('Error fetching data:', error);
        });
    };

    const intervalId = setInterval(fetchData, 5000);

    return () => clearInterval(intervalId);
  }, []);

  const handleNavigate = (page) => {
    setCurrentPage(page);
  };

  if (currentPage === 'main') {
    return (
      <div style={{ backgroundColor: '#f0f0f5', padding: '20px', minHeight: '50vh' }}>
        <h1 style={{ textAlign: 'center', marginBottom: '20px' }}>COOL GUARD</h1>
        <button onClick={() => handleNavigate('second')}>ESP PIN</button>
        <button onClick={() => handleNavigate('second')}>Data Logger</button>
        {tampilantabel(data)}
        
      </div>
    );
  } else if (currentPage === 'second') {
    return (
      <div>eeee


<SecondPage onNavigate={() => handleNavigate('main')} />
      </div>
     
      
    );
    
  }
   
}

function tampiltabel(data) {
  return data.map(item => (
    <div
      key={item.id}
      style={{
        border: '1px solid #ddd',
        padding: '10px',
        margin: '10px 0',
        borderRadius: '10px',
        boxShadow: '0 2px 5px rgba(0, 0, 0, 0.1)',
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
                    backgroundColor: item[key] === 0 ? 'blue' : 'red',
                    padding: '5px',
                    borderRadius: '5px',
                    color: 'white',
                  }}
                >
                  {item[key]}
                </div>
              ) : (
                (key.includes('img') || key === 'photo') ? (
                  <img
                    id={key}
                    src={item[key]}
                    alt={key}
                    style={{
                      width: '100px',
                      height: '100px',
                      borderRadius: key === 'img' ? '50%' : '0',
                      objectFit: 'cover',
                    }} />
                ) : key === 'timestamp' ? (
                  <TimeDifferenceMenu timestamp={item[key]} />
                ) : (
                  item[key]
                )
              )}
            </div>
          </div>
        )
      ))}
    </div>
  ));
}
function tampilantabel(data) {
  return data.map(item => (
    <div
      key={item.id}
      style={{
        border: '1px solid #ddd',
        padding: '10px',
        margin: '10px 0',
        borderRadius: '10px',
        boxShadow: '0 2px 5px rgba(0, 0, 0, 0.1)',
        backgroundColor: '#eeff',
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
                    backgroundColor: item[key] === 0 ? 'blue' : 'red',
                    padding: '5px',
                    borderRadius: '5px',
                    color: 'white',
                  }}
                >
                  {item[key]}
                </div>
              ) : key.includes('PIN') ? ( // Penanganan khusus untuk kunci yang mengandung "PIN"
              
              <div 
                 style={{display: 'flex',
    backgroundColor: item[key] === 0 ? 'blue' : 'red', // Kondisi untuk latar belakang
    color: 'white', // Warna teks putih agar kontras dengan latar belakang
    padding: '5px', // Tambahkan padding agar teks tidak terlalu mepet dengan tepi
    borderRadius: '5px', // Tambahkan border radius untuk tampilan yang lebih halus
  }}>

                  {item[key]}
                  
                </div>
                
              ) : (
                (key.includes('img') || key === 'photo') ? (
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
                ) : (
                  item[key]
                )
              )}
            </div>
          </div>
        )
      ))}
    </div>
  ));
}



function LedArray({ decimalValue }) {
  // Konversi desimal ke biner 4-bit
  const binary = (decimalValue >>> 0).toString(2).padStart(4, '0');
  const bits = binary.split('').map(Number);

  return (
    <div style={{ display: 'flex' }}>
      {bits.map((bit, index) => (
        <div
          key={index}
          style={{
            width: '20px',
            height: '20px',
            borderRadius: '50%',
            backgroundColor: bit === 1 ? 'red' : 'gray',
            margin: '5px',
            boxShadow: bit === 1 ? '0 0 10px red' : '0 0 5px rgba(0, 0, 0, 0.5)',
          }}
        />
      ))}
    </div>
  );
}

function TimeDifferenceMenu({ timestamp }) {
  const isMilliseconds = String(timestamp).length > 10;
  const timestampInSeconds = isMilliseconds ? Math.floor(timestamp / 1000) : timestamp;
  const currentTime = Math.floor(Date.now() / 1000);
  const timeDiff = currentTime - timestampInSeconds;

  const formatTimeDiff = (diff) => {
    if (diff < 60) {
      const detik = Math.floor(diff);
      return `${detik} detik yang lalu`;
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

function SecondPage({ onNavigate }) {
  return (
    <div style={{ textAlign: 'center', padding: '50px' }}>
      <h1>Data Pin ESP</h1>
      <LedArray decimalValue={2} />
  <button onClick={onNavigate}>Back</button>
      <ul><img src="https://github.com/ninofelino12/react-router-project/blob/master/img/PinOut-NodeMCU-V.1.png?raw=true"/>
      <ul><img src="https://raw.githubusercontent.com/ninofelino12/react-router-project/refs/heads/master/img/ESP8266-LED-pushbutton_bb.webp"/></ul>
       <li>D1-SCL lcd</li>
       <li>D2-SDA </li>
       <a href="https://raw.githubusercontent.com/ninofelino12/react-router-project/refs/heads/master/img/circuit-diagram-10.png">LCD</a>
       <li>D3 - LCD</li>
       <ul><img src="https://raw.githubusercontent.com/ninofelino12/react-router-project/refs/heads/master/img/esp8266_LCD.webp"/></ul>
       <li>D4 - LCD</li>
       <li>D5 - BUTTON</li>
       <li>D6 - BUTTON</li>
       <li>D7 - BUTTON</li>
     </ul>
    
    </div>
     
     
  );
}

export default App;