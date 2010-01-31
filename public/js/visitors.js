var page;
if(!page)
{
	page = {
		//page init function
		init: function ()
		{
			this.chart.getDataPoints = function (it)
			{
				var i, points = [];
				
				if(it === 0)
				{
					for(i = 0; i < this.chartData.length; i++)
					{		
						points.push([i, this.chartData[i]]);
					}
					
					return points;
				}
				else
				{
					return [];
				}
			}
			
			this.chart.getPoints = function (it)
			{
				if(it === 0)
				{
					return { show: true, radius: 3};
				}
				else
				{
					return { show: false};
				}
			}
			
			this.chart.getBars = function (it)
			{
				return {show: false};
			}
			
			this.chart.getLines = function (it)
			{	
				if(it === 0)
				{
					return {show: true, lineWidth: 4};
				}
				else
				{
					return {show: false};
				}
			}
			
			this.chart.setSettings({
				legend: { noColumns: 3, container: $("#chart-legend")},
				colors: ["#7297BA"],
				yaxis: {min: 0, tickDecimals: 0},
				xaxis: {tickDecimals: 0},
				grid: {borderWidth: 0, tickColor: "#ccc", backgroundColor: {colors: ["#fff", "#ddd"]}}
			});
			
			this.chart.setSeries(["visits"]);
			
			this.chart.updatePoints([10, 12, 13, 14, 11, 11, 9]);
			
			this.chart.setCanvas($("#visitors-chart"));
			
			this.chart.draw();
		},
		
		chart: new Chart(30, "day")
	};
}

$(function (){
	page.init();		
});
