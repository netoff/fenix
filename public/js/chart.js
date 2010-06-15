function Chart(n, resolution)
{
	var t = [],i;
	for(i = 0; i < n; i++){t.push([0]);};
	this.chartData = t;
	
	this.numPoints = n;
	
	this.resolution = resolution; //sec, min...
	
	this.series = []; //"new visitors", "returning visitors"...
}

Chart.prototype.setNumPoints = function (n)
{
	this.numPoints = n;
}
Chart.prototype.setSeries = function (s)
{
	this.series = s;
}

Chart.prototype.updatePoints = function (points)
{
	var i, a;
	
	for (i = 0; i < points.length; i++)
	{
		this.chartData.push(points[i]);
		if (this.chartData.length > this.numPoints + 1) 
		{
			this.chartData.shift();
		}
	}
		
};

Chart.prototype.setPoints = function (points)
{
	this.numPoints = points.length;
	this.chartData = points;
}

Chart.prototype.setCanvas = function (canvas) 
{ 
	this.canvas = canvas;
};
Chart.prototype.setSettings = function (settings) 
{ 
	this.settings = settings;
};
Chart.prototype.draw = function ()
{
	var i, chart_data = [], n = this.series.length, label;
	
	for(i = 0; i < n; i++ )
	{
		if(this.series[i] !== "")
		{
			label = this.series[i] + "/" + this.resolution;
		}
		else
		{
			label = "";
		}
		chart_data.push({ color: this.getColor(i, n), 
				data: this.getDataPoints(i, n), 
				points: this.getPoints(i, n),
				bars: this.getBars(i, n), 
				lines: this.getLines(i, n),
				stack: this.getStack(i, n),
				label:  label});
	}
	
	$.plot(this.canvas, chart_data, this.settings);
	
	var previousPoint = null;
	var chart = this;
	
	this.canvas.bind("plothover", function (event, pos, item) 
	{
		if (item) 
		{			
			if (previousPoint != item.datapoint) 
			{
			    previousPoint = item.datapoint;
			    
			    $("#tooltip").remove();
			 
			    chart.showTooltip(item.pageX, item.pageY, chart.tooltipFormat(item, chart.series));
			}
		}
		else 
		{
			$("#tooltip").remove();
			
			previousPoint = null;            
		}		
	});
};
Chart.prototype.showTooltip = function (x, y, contents) 
{
        $('<div id="tooltip">' + contents + '</div>').css({
			display: 'none',
			top: y + 5,
			left: x + 15			
        }).appendTo("body").fadeIn(200);
};



