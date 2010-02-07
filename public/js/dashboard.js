/*global $ */

function Chart()
{
	this.chartData = [];
	
	this.maxVal = 10;
	this.numPoints = 60;
	
	this.resolution = ""; //sec, min....
	
	this.maxValues = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 150, 200, 250, 300, 400, 500, 600, 700, 800, 900, 1000];
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

Chart.prototype.findMax = function (x, max)
{
	var i, maxI;
	
	if (x <= max) 
	{
		return max;
	}
	
	for (i = 0; i < this.maxValues.length; i++)
	{
		maxI = this.maxValues[i];
		
		if (x <= maxI) 
		{
			return maxI;
		}
	}
	
	return this.maxValues[this.maxValues.length - 1];
};

Chart.prototype.getDataPoints = function (it)
{
	var i, a, diff, points = [];
	
	diff = this.numPoints - this.chartData.length;
	
	for (i = 0; i < this.numPoints; i++)
	{
		if (i >= diff)
		{
			a = [i, this.chartData[i - diff][it]];
		}
		else
		{
			a = [i, 0];
		}
			
		points.push(a);
	}	
	
	return points;
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
	
	$.plot(this.canvas, [
		{ data: this.getDataPoints(1), bars: { show: true, barWidth: 0.7, align: "center" }, label: "new visitors/" + this.resolution, stack: true },
		{ data: this.getDataPoints(2), bars: { show: true, barWidth: 0.7, align: "center" }, label: "returning visitors/" + this.resolution, stack: true},
		{ data: this.getDataPoints(0), lines: { show: true, steps: true }, points: { show: true, radius: 1}, label: "page hits/" + this.resolution }], this.settings);

	
};

var dashboard;
if (!dashboard) 
{
	dashboard = (function ()
	{
		return {
			site_id: "nx3456",
			
			timestamp: 0,
			timestampMin: 0,
	
			init: function ()
			{
				this.chart.setCanvas($('#trafic-live-chart'));
				this.chart.resolution = "sec";
				this.chart.settings = { 
					legend: { noColumns: 3, container: $('#chart-legend')},
					colors: ["#FF9900", "#7297BA", "#6f3"],
					yaxis: { min: 0, tickDecimals: 0},
					xaxis: { ticks: [[0, "1 min ago"], [9, ""], [19, ""], [29, "30 secs ago"], [39, ""], [49, ""], [59, "1 sec ago"]]},
					grid: { borderWidth: 0, tickColor: "#ccc", backgroundColor: {colors: ["#fff", "#ddd"]}}
				};
				
				this.chart.draw();
		
				this.chartMin.setCanvas($('#trafic-live-chart1'));
				this.chartMin.resolution = "min";
				this.chartMin.settings = { 
					legend: { noColumns: 3, container: $('#chart1-legend')},
					crosshair: { mode: "x"},
					colors: ["#FF9900", "#7297BA", "#6f3"],
					yaxis: { min: 0, tickDecimals: 0},
					xaxis: { ticks: [[0, "1 h ago"], [9, ""], [19, ""], [29, "30 mins ago"], [39, ""], [49, ""], [59, "1 min ago"]]},
					grid: {borderWidth: 0, tickColor: "#ccc", backgroundColor: {colors: ["#fff", "#ddd"]}}
				};				
				
				this.chartMin.draw();
				
				this.sameHeight($("div.same-height"));
			},
	
			sameHeight: function (sections)
			{
				var max, i, j, section;
		
				for (i = 0; i < sections.length; i++)
				{
					max = 0;
					
					section = $("div.window", sections[i]);
			
					for (j = 0; j < section.length; j++)
					{
						if ($(section[j]).height() > max)
						{
							max = $(section[j]).height();
						}
					}

					for (j = 0; j < section.length; j++)
					{
						$(section[j]).height(max);
					}
				}
			},
	
			update: function () 
			{
				$.getScript('/app/stats/views?id=' + this.site_id + '&t=' + this.timestamp + '&t1=' + this.timestampMin);
			},
	
			chart: new Chart(),
			chartMin: new Chart(),
			
			updateData: function ()
			{
				var points, a, l;
				
				points = this.chart.chartData;
				l = points.length;
				
				a = points[l - 1];
				
				$("#stat-hps").html(a[0]);
				$("#stat-nvps").html(a[1]);
				$("#stat-rvps").html(a[2]);
			},
			
			updateDataMin: function ()
			{
				var points, a, l;
				
				points = this.chartMin.chartData;
				l = points.length;
				
				a = points[l - 1];
				
				$("#stat-hpm").html(a[0]);
				$("#stat-nvpm").html(a[1]);
				$("#stat-rvpm").html(a[2]);
			}
		};
	}());
}

$(function ()
{
	dashboard.init();

	//Start the loop
	dashboard.update();	
});