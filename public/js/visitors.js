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
				
				return [];
			}
			
			this.chart.getPoints = function (it)
			{
				if(it === 0)
				{
					return { show: true, radius: 3};
				}
				
				return { show: false };
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
					
				return {show: false};
			}
			
			this.chart.setSettings({
				legend: { noColumns: 3, container: $("#chart-legend")},
				colors: ["#7297BA"],
				yaxis: {min: 0, tickDecimals: 0},
				xaxis: {tickDecimals: 0},
				grid: {borderWidth: 0, tickColor: "#ccc", backgroundColor: {colors: ["#fff", "#ddd"]}}
			});
			
			this.chart.setSeries(["visits"]);
			
			//this.chart.updatePoints([10, 12, 13, 14, 11, 11, 9]);
			
			this.chart.setCanvas($("#visitors-chart"));
			
			this.chart.draw();
			
			$("#date-range-selector-input").DatePicker({
					mode: 'range',
					calendars: 2,
					format: 'm/d/Y',
					date: ['01/11/2010', '02/10/2010']
			});
		},
		
		chart: new Chart(30, "day"),
		
		update: function ()
		{
			var date_range = $("#date-range-selector-input").val();
			
			if(date_range)
			{
				
			}
		}
	};
}

$(function (){
	page.init();
	page.update();
	
	$("#date-range-selector-submit").click(function ()
		{
			page.update();
			
			return false;
		});
});
