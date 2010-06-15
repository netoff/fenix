$(function (){
	page.init(function(page){
			page.setEndpoint("/analytics/poll/visitors");
			
			var visits = $('#stat-vs'), visits_per_day =$('#stat-vspd'),
					new_visitors = $('#stat-nv'), new_visitors_per_day = $('#stat-nvpd'),
					visitors_list_by_country = $('#visitors-segments');
					
			function updateOverview(overview)
			{
				if(overview && overview.length > 3)
				{
					visits.html(overview[0]);
					visits_per_day.html(overview[1]);
					new_visitors.html(overview[2]);
					new_visitors_per_day.html(overview[3]);					
				}
			};
			
			function updateSegments(segments)
			{				
				if(segments)
				{
					var countries = segments.countries,
							len = countries.length;
				
					if(countries && len > 0)
					{	
						page.updateSegmentedList(visitors_list_by_country, ["#", "Country", "Visits", "New Visitor", "%New"], len,
							function(i){
								var row = [];
							
								if(i < len)
								{	
									var country = countries[i];
									row.push(i+1);row.push("<strong>" + lookupCountry(country.name) + "</strong>");row.push(country.visits);
									row.push(country.new_visits);row.push(country.percent);
								}
							
								return row;
						});
					}
					else
					{
						visitors_list_by_country.html("<p>No data available.</p>");
					}					
				}				
			};
			
			page.updateFromFeed = function (feed)
			{
				if(feed)
				{
					var points = feed.points,
							type = feed.type,
							overview = feed.overview,
							segments = feed.by;
							
					if(points && points.length > 0 && type && overview && segments)
					{
						var series = [];
						if(type === "-1") {series = ['new visitors', 'returning visitors', 'repeating visitors', 'visits'];}
						if(type === "2") {series = ['new visitors'];}
						if(type === "3") {series = ['returning visitors'];}
						if(type === "4") {series = ['repeating visitors'];}
						
						page.chart.setSeries(series);
						page.chart.setPoints(points);
						page.chart.draw();
						
						updateOverview(overview);
						updateSegments(segments);
					}
				}
			};
	});
});
