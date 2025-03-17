import React, { useEffect, useState } from 'react';
import axios from 'axios';

const DataTable = () => {
    const [data, setData] = useState([]);

    useEffect(() => {
        axios.get('https://myiot-5a297-default-rtdb.asia-southeast1.firebasedatabase.app/.json?orderBy=%22$key%22&startAt=%22master%22&endAt=%22master\\uf8ff%22')
            .then(response => {
                setData(Object.values(response.data));
            })
            .catch(error => {
                console.error('Error fetching data:', error);
            });
    }, []);

    return (
        <table>
            <thead>
                <tr>
                    <th>Key</th>
                    <th>Value</th>
                </tr>
            </thead>
            <tbody>
                {data.map((item, index) => (
                    <tr key={index}>
                        <td>{item.key}</td>
                        <td>{item.value}</td>
                    </tr>
                ))}
            </tbody>
        </table>
    );
};

export default DataTable;