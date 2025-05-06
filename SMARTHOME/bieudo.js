let myChart1; 
document.addEventListener('DOMContentLoaded', function() {
    const ctx = document.getElementById('myChart1').getContext('2d');
    myChart1 = new Chart(ctx, {
        type: 'bar',
        data: {
            labels: ['', '', '', '', '','',''],
            datasets: [
                {
                    label: 'Nhiệt độ',
                    data: [0, 0, 0, 0, 0, 0, 0],  // Giá trị nhiệt độ cho mỗi ngày
                    backgroundColor: '#ffcc00',  // Màu vàng cho nhiệt độ
                    borderColor: '#ff9900',  // Màu viền cam cho nhiệt độ
                    borderWidth: 1,
                    barThickness: 40,  // Độ dày của cột
                    categoryPercentage: 0.3,  // Tỉ lệ của nhóm cột
                    barPercentage: 0.8,  // Tỉ lệ độ rộng của cột trong nhóm
                },
                {
                    label: 'Độ ẩm',
                    data: [0, 0, 0, 0, 0, 0, 0],  // Giá trị độ ẩm cho mỗi ngày
                    backgroundColor: '#36a2eb',  // Màu xanh cho độ ẩm
                    borderColor: '#2e89c8',  // Màu viền xanh đậm cho độ ẩm
                    borderWidth: 1,
                    barThickness: 40,
                    categoryPercentage: 0.3,
                    barPercentage: 0.8,
                },
                {
                    label: 'Ánh sáng',
                    data: [0, 0, 0, 0, 0, 0, 0],  // Giá trị ánh sáng cho mỗi ngày
                    backgroundColor: '#32cd32',  // Màu xanh lá mạ cho ánh sáng
                    borderColor: '#228b22',  // Màu viền xanh lá đậm cho ánh sáng
                    borderWidth: 1,
                    barThickness: 40,
                    categoryPercentage: 0.3,
                    barPercentage: 0.8,
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,  // Cho phép thay đổi tỉ lệ của biểu đồ khi thay đổi kích thước
            scales: {
                y: {
                    beginAtZero: true,
                    ticks: {
                        stepSize: 10,  // Điều chỉnh bước của các giá trị trục y
                    }
                }
            },
            plugins: {
                legend: {
                    position: 'top',  // Đặt vị trí legend ở trên
                },
                tooltip: {
                    backgroundColor: 'rgba(0, 0, 0, 0.7)',  // Màu nền của tooltip
                    titleColor: '#fff',  // Màu chữ của title trong tooltip
                    bodyColor: '#fff',   // Màu chữ của nội dung trong tooltip
                }
            },
            // Điều chỉnh khoảng cách giữa các nhóm cột
            datasets: {
                bar: {
                    barPercentage: 0.8,  // Điều chỉnh độ rộng cột trong nhóm
                    categoryPercentage: 0.4,  // Điều chỉnh khoảng cách giữa các nhóm cột
                }
            }
        }
    });

        const mqttClient = new Paho.MQTT.Client('192.168.180.26', 8080, 'clientId' + new Date().getTime());
        mqttClient.connect({
            userName: 'qwer',
            password: '123',
            onSuccess: function() {
                console.log("Connected to MQTT Broker");
                
            },
            onFailure: function(responseObject) {
                console.error("Connection failed: " + responseObject.errorMessage);
            }
        });
        function publishMessage(ledStatus) {
            const message = new Paho.MQTT.Message(JSON.stringify(ledStatus)); 
            message.destinationName = "led"; 
            mqttClient.send(message);
            console.log("Message to topic " + "led" + " is sent: ", ledStatus);
        }
        const lights = document.querySelectorAll('.toggle-light');
        const switchStatuses = document.querySelectorAll('.switch-status');
        const lightBulbs = document.querySelectorAll('.light-bulb');

        lights.forEach((toggleSwitch, index) => {
            toggleSwitch.addEventListener('change', async function(event) {
                if (switchStatuses[index] && lightBulbs[index]) {
                    const id = event.target.id
                    const status = this.checked ? 'ON' : 'OFF';
                    switchStatuses[index].textContent = status;
                    lightBulbs[index].src = this.checked ? 
                        'https://img.icons8.com/ios-filled/50/000000/light-on.png' : 
                        'https://img.icons8.com/ios-filled/50/000000/light-off.png';
        
                    const ledStatus = {
                        [`led${index + 1}`]: this.checked ? 'on' : 'off'
                    };
                    publishMessage(ledStatus);
                } else {
                    console.error("Switch status or light bulb not found for index:", index);
                }
            });
        });
    
    const toggleFan = document.getElementById('toggle-fan');
    const switchStatusFan = document.getElementById('switch-status-fan');
    const fanIcon = document.getElementById('fan-icon');

    toggleFan.addEventListener('change', function() {
        if (this.checked) {
            switchStatusFan.textContent = 'ON';
            fanIcon.style.animation = 'spin 2s linear infinite';
        } else {
            switchStatusFan.textContent = 'OFF';
            fanIcon.style.animation = 'none'; 
        }
    });

    const style = document.createElement('style');
    style.innerHTML = `
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
    `;
    document.head.appendChild(style);
});

let timeIndex1 = 0;
let timeIndex2 = 0;

const maxDataPoints = 7; 

function updateChart1(newTemperature, newHumidity, newLight) {
    if (timeIndex1 >= maxDataPoints) {
        myChart1.data.datasets[0].data = [0, 0, 0, 0, 0, 0, 0]; 
        myChart1.data.datasets[1].data = [0, 0, 0, 0, 0, 0, 0]; 
        myChart1.data.datasets[2].data = [0, 0, 0, 0, 0, 0, 0]; 
        myChart1.data.labels = ['', '', '', '', '','','']; 
        timeIndex1 = 0; 
    }

    const currentTime = new Date().toLocaleTimeString(); 

    myChart1.data.labels[timeIndex1] = currentTime;

    myChart1.data.datasets[0].data[timeIndex1] = newTemperature;  
    myChart1.data.datasets[1].data[timeIndex1] = newHumidity;  
    myChart1.data.datasets[2].data[timeIndex1] = newLight;         

    timeIndex1++;

    myChart1.update();
}    
    function fetchSensorData() {
        fetch('http://192.168.180.26:5000/api/sensor-data', {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            }
        })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            console.log('Data fetched from backend:', data);
            if (data) {
                temperatureString = data.temperature + "C";
                humidityString = data.humidity + "%";
                lightString = data.light + "lux";
                document.getElementById("temperature").innerText = temperatureString;
                document.getElementById("humidity").innerText = humidityString;
                document.getElementById("light").innerText = lightString;

                const newTemperature = parseFloat(data.temperature);
                const newHumidity = parseFloat(data.humidity);
                const newLight = parseFloat(data.light);
                updateChart1(newTemperature, newHumidity, newLight);
                
            }
        })
        .catch((error) => {
            console.error('Error fetching data from backend:', error);
        });
}

window.onload = function() {
    fetchSensorData();
    setInterval(fetchSensorData, 10000);
};


function switchTab(tabId) {
    const activeTab = document.querySelector('.tab.active');
    if (activeTab) {
        activeTab.classList.remove('active');
    }

    const activeSidebarTab = document.querySelector('.table.active');
    if (activeSidebarTab) {
        activeSidebarTab.classList.remove('active');
    }
    const selectedSidebarTab = document.querySelector(`.table[onclick="switchTab('${tabId}')"]`);
    if (selectedSidebarTab) {
        selectedSidebarTab.classList.add('active');
    }

    const selectedTab = document.getElementById(tabId);
    if (selectedTab) {
        selectedTab.classList.add('active');
    }
}

document.addEventListener('DOMContentLoaded', () => {
    switchTab('tab1');  
});

