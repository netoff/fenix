$(function(){
		page.init(function(page){
				page.setEndpoint("/analytics/poll/referrers");
				
				var visits = $("#stat-vs"), visits_per_day = $("#stat-vspd"),
						new_visits = $("#stat-nv"), new_visits_per_day = $("#stat-nvpd"),
						referrers_list_by_ref = $("#referrers-segments");				
				
				function updateOverview(overview)
				{
					if(overview && overview.length > 3)
					{
						visits.text(formatInteger(overview[0]));
						visits_per_day.text(formatFloat(overview[1]));
						new_visits.text(formatInteger(overview[2]));
						new_visits_per_day.text(formatFloat(overview[3]));
					}
				}
				function updateSegments(segments)
				{
					if(segments)
					{
						var referrers = segments.referrers,
								len = referrers.length, a, referrer;
						
						if(referrers && len > 0)
						{	
							page.updateSegmentedList(referrers_list_by_ref, ["#", "Referrer", "Visits", "New Visits", "% New"], len,
								function(i){
									a = [];
									referrer = referrers[i];
									a.push(i + 1); a.push("<strong>"+referrer.name+"</strong>"); 
									a.push(formatInteger(referrer.visits));
									a.push(formatFloat(referrer.new_visits));
									a.push(formatPercent(referrer.percent));
									return a;
							});
						}
						else
						{
							referrers_list_by_ref.html("<p>No data available.</p>");
						}
					}				
				}
				
				page.updateFromFeed = function(feed)
				{
					if(feed)
					{
						var points = feed.points,
								type = feed.type,
								overview = feed.overview,
								segments = feed.by, series = [];
						if(points && points.length > 0 && type && overview && segments)
						{
							if(type === "-1"){series = ["direct", "search engines", "referred", "all traffic"];};
							if(type === "2"){series = ["direct"];};
							if(type === "3"){series = ["search engines"];};
							if(type === "4"){series = ["referred"];};
							
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
