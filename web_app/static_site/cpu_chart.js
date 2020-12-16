let config = {
    type: 'line',
    data: {
        labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July'],
        datasets: [{
            label: '%lol',
            fill: false,
            backgroundColor: "cyan",
            borderColor: "cyan",
            data: [
                70,
                70,
                70,
                70,
                70,
                70,
                70
            ],
        }]
    },
    options: {
        responsive: true,
        plugins: {
            title: {
                display: true,
                text: 'Chart.js Line Chart'
            },
            tooltip: {
                mode: 'index',
                intersect: false,
            }
        },
        hover: {
            mode: 'nearest',
            intersect: true
        },
        scales: {
            x: {
                display: true,
                scaleLabel: {
                    display: true,
                    labelString: 'Month'
                }
            },
            y: {
                display: true,
                scaleLabel: {
                    display: true,
                    labelString: 'Value'
                }
            },
            yAxes: [{
                ticks: {
                    beginAtZero: true,
                    suggestedMin: 0,
                    suggestedMax: 100
                }
            }]
        }
    }
};

window.onload = function() {
    var ctx = document.getElementById('cpu_usage').getContext('2d');
    window.myLine = new Char(ctx, config);
};