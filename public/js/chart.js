function Chart(n, resolution)
{
	this.chartData = [];
	
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
	var i, chart_data = [];
	
	for(i = 0; i < this.series.length; i++ )
	{
		chart_data.push({ data: this.getDataPoints(i), 
				points: this.getPoints(i),
				bars: this.getBars(i), 
				lines: this.getLines(i), 
				label: this.series[i] + "/" + this.resolution });
	}
	
	$.plot(this.canvas, chart_data, this.settings);
};


