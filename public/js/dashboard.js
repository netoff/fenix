var timestamp;
var chartData = [];
var chartPlot;
var maxVal = 10;
var maxValMid = 5;

var chartWidth ;
var chartHeight;
var unitWidth;

const chartMarginLeft = 40;
const chartMarginBottom = 20;
const chartMarginTop = 5;
const numPoints = 59;
const maxValues = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 150, 200, 300, 400, 500, 600, 700, 800, 900, 1000]

function findMax(x, max)
{
	if(x <= max) return max;
	
	for(var i = 0; i < maxValues.length; i++)
	{
		var maxI = maxValues[i];
		
		if(x <= maxI) return maxI;
	}
	
	return maxValues[maxValues.length - 1];
}

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

function getStart()
{
	if(chartData)
		return (numPoints + 1 - chartData.length) * unitWidth;
	else 
		return 0;
}

function getX(x)
{
	var ret = chartMarginLeft + x * unitWidth;
	return parseInt(ret);
}

function getY(y, max)
{
	var height = chartHeight - chartMarginBottom - chartMarginTop;
	var ret = chartMarginTop + (height - height/max * y);
	
	return parseInt(ret);
}

function drawLine(x, y, x1, y1, color, plot)
{
	plot.strokeStyle = color;
	plot.lineWidth = 1;
	
	chartPlot.beginPath();
	
	plot.moveTo(parseInt(x), parseInt(y));
	plot.lineTo(parseInt(x1), parseInt(y1));	
	
	chartPlot.stroke();
}

function drawGrid()
{
	chartPlot.save();
	
	drawLine(chartMarginLeft, chartMarginTop, chartMarginLeft, chartHeight- chartMarginBottom, "#333", chartPlot);
	
	drawLine(chartMarginLeft - 20, chartMarginTop, chartMarginLeft, chartMarginTop, "#333", chartPlot);
	drawLine(chartMarginLeft, chartMarginTop, chartWidth, chartMarginTop, "#ccc", chartPlot);
	
	var midline = chartMarginTop + (chartHeight - chartMarginBottom - chartMarginTop)/2;
	
	drawLine(chartMarginLeft - 20, midline, chartMarginLeft, midline, "#333", chartPlot);
	drawLine(chartMarginLeft, midline, chartWidth, midline, "#ccc", chartPlot);
	drawLine(chartMarginLeft, chartHeight - chartMarginBottom, chartWidth, chartHeight - chartMarginBottom, "#333", chartPlot);
	
	drawLine(getX(0), chartHeight - chartMarginBottom, getX(0), chartHeight - chartMarginBottom + 7, "#333", chartPlot);
	for(i = 1; i < numPoints + 1; i++)
	{
		var x = getX(i);
		drawLine(x, getY(1, 1), x, getY(0, 1), "#ccc", chartPlot);
		
		var l = 3;
		if((i + 1) %15 == 0) l = 7;  
		drawLine(x, chartHeight - chartMarginBottom, x, chartHeight - chartMarginBottom + l, "#333", chartPlot);
	}
	chartPlot.restore();
}

function drawChart()
{    
	chartPlot.clearRect(0, 0, chartWidth, chartHeight);
	
	drawGrid();
	
	chartPlot.save();
	
    chartPlot.strokeStyle = "#080";
    
    chartPlot.lineWidth = 3;
    chartPlot.lineJoin = "round";
    chartPlot.beginPath();
  
  	if(chartData.length > 0)
  	{
  		//First you have to find max value then, draw chart
  		
  		maxVal = 10;
  		maxValMid = 5;
  		
  		for (var i=0; i < chartData.length; i++) 
    	{
    		if(chartData[i] > maxVal)
			{
				maxVal = findMax(chartData[i], maxVal);
				maxValMid = maxVal/2;
			}
    	}
    	
    	$("#chart-max").html(maxVal);
    	$("#chart-max_mid").html(maxValMid);
  	
    	chartPlot.moveTo(getStart() + getX(0), getY(chartData[0], maxVal));
    	for (var i=0; i < chartData.length; i++) 
    	{
			chartPlot.lineTo(getStart() + getX(i), getY(chartData[i], maxVal));
    	}
    	
    	$("#stat-vps").html(chartData[chartData.length - 1]);
    }
    
    chartPlot.stroke();
    
    chartPlot.restore();
}



function updateDashboard()
{
	$.getScript('/app/stats/views?t=' + timestamp);
}

function sameHeight(sections)
{
	for(var i = 0; i < sections.length; i++)
	{
		var max = 0;
		

		var section = $(sections[i]).find("div.window");
		
		for(var j = 0; j < section.length; j++)
		{
			if($(section[j]).height() > max)
				max = $(section[j]).height();
		}

		for(var j = 0; j < section.length; j++)
			$(section[j]).height(max);
	}
}
$(document).ready(function()
{
	sameHeight($("div.same-height"));
	
	timestamp = 0;
	
	var canvas = $('#trafic-live-chart');
	
	chartPlot = canvas.get(0).getContext('2d');
	
	chartWidth = canvas.width();
	chartHeight = canvas.height();
	unitWidth = (chartWidth - chartMarginLeft)/numPoints;
	
	updateDashboard();	
});