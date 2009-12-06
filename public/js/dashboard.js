var timestamp;
var chartData = [];
var chartPlot;
const chartWidth = 686;
const chartHeight = 200;
const chartMarginLeft = 40;
const chartMarginBottom = 20;
const chartMarginTop = 5;
const numPoints = 60;

function updatePoints(points)
{
	for(var i = 0; i < points.length; i++)
	{
		chartData.push(points[i]);
		if (chartData.length > numPoints + 1) 
		{
      		chartData.shift();
    	}
	}
}

function getX(x)
{
	return chartMarginLeft + x *(chartWidth - chartMarginLeft)/numPoints;
}

function getY(y, max)
{
	return chartMarginTop + (chartHeight - chartMarginBottom - chartMarginTop)/max * y;
}

function drawGrid()
{
	chartPlot.save();
	chartPlot.strokeStyle = "#ccc";
	chartPlot.lineWidth = 1;
	
	chartPlot.beginPath();
	
	chartPlot.moveTo(chartMarginLeft, chartMarginTop);
	chartPlot.lineTo(chartMarginLeft, chartHeight - chartMarginBottom);
	chartPlot.moveTo(chartMarginLeft, chartMarginTop);
	chartPlot.lineTo(chartWidth, chartMarginTop);
	chartPlot.moveTo(chartMarginLeft, chartMarginTop + (chartHeight - chartMarginBottom - chartMarginTop)/2);
	chartPlot.lineTo(chartWidth, chartMarginTop + (chartHeight - chartMarginBottom - chartMarginTop)/2);
	chartPlot.moveTo(chartMarginLeft, chartHeight - chartMarginBottom);
	chartPlot.lineTo(chartWidth, chartHeight - chartMarginBottom);
	chartPlot.stroke();
	
	chartPlot.restore();
}

function drawChart()
{
	var maxVal=10;
	
    for (var i=0; i < chartData.length; i++)
      if (chartData[i] > maxVal) maxVal=chartData[i];

	chartPlot.clearRect(0, 0, chartWidth, chartHeight);
	
	drawGrid();
	
	chartPlot.save();
	
    chartPlot.strokeStyle = "#080";
    
    chartPlot.lineWidth = 3;
    chartPlot.lineJoin = "round";
    chartPlot.beginPath();
  
    //chartPlot.moveTo(0, 90 - (80.0*chartData[0]/maxVal));
    chartPlot.moveTo(getX(0), getY(chartData[0], maxVal));
    for (var i=1; i < chartData.length; i++) 
    {
      //chartPlot.lineTo( 10*i, 90-(80.0*chartData[i]/maxVal) );
      chartPlot.lineTo(getX(i), getY(chartData[i], maxVal));
    }
    
    chartPlot.stroke();
    
    chartPlot.restore();
}



function updateDashboard()
{
	$.getScript('/app/stats/views?t=' + timestamp);
}

$(document).ready(function()
{
	timestamp = 0;
	chartPlot = $('#trafic-live-chart').get(0).getContext('2d');
	
	updateDashboard();	
});