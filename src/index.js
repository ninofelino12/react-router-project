import React from 'react';
import ReactDOM from 'react-dom/client';
import './index.css';
import App from './App';
import reportWebVitals from './reportWebVitals';

const root = ReactDOM.createRoot(document.getElementById('root'));


    root.render(
      <React.StrictMode>
        <App />
        <div id="data"></div>

    <DataDisplay /> {/* Render komponen DataDisplay */}
      </React.StrictMode>
      
    );

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
reportWebVitals();

function DataDisplay() {
  return (
    <div id="datan">
      DATA PIN ESP8266
      <ul>
        <li>D1-SCL lcd/li>
        <li>D2-SDA </li>
        <a href="https://raw.githubusercontent.com/ninofelino12/react-router-project/refs/heads/master/img/circuit-diagram-10.png">LCD</a>
        <li>D3 - LCD</li>
        <li>D4 - LCD</li>
        <li>D5 - BUTTON</li>
        <li>D6 - BUTTON</li>
        <li>D7 - BUTTON</li>
      </ul>
    </div>
  );
}
