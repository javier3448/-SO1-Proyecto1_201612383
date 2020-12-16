let ram_ctx = document.getElementById("ram_usage").getContext("2d");

let ram_chart = new Chart(ram_ctx, {
    type: 'line',
    data: {
        labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July'],
        datasets: [{
            label: '%ram',
            fill: false,
            backgroundColor: "#fdad5c",
            borderColor: "#fdad5c",
            data: [
                50,
                50,
                50,
                50,
                50,
                50,
                50
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
});