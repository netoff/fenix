$(function(){
		page.init(function(page){
				page.setEndpoint("/analytics/poll/pages");
				
				var hits = $("#stat-h"),
						hits_per_day = $("#stat-hpd"),
						pages_list_by_url = $("#pages-segments"); 
						
				function updateOverview(overview)
				{
					if(overview && overview.length > 1)
					{
						hits.html(overview[0]);
						hits_per_day.html(overview[1]);
					}
				}
				function updateSegments(segments)
				{
					if(segments)
					{
						var pages = segments.pages,
								len = pages.length, p, a;
						
						if(pages && len > 0)
						{
								
							page.updateSegmentedList(pages_list_by_url, ["#", "Url", "Hits"], len, 
								function(i){
									p = pages[i];
								
									a = [];
									a.push(i + 1);a.push("<strong>"+p.name+"</strong>");a.push(p.hits);
									
									return a;
							});
						}
						else
						{
							pages_list_by_url.html("<p>No data available.</p>");
						}
					}
					
				}				
				
				page.updateFromFeed = function (feed)
				{
					if(feed)
					{						
						var points = feed.points,
								overview = feed.overview,
								type = feed.type,
								segments = feed.by,
								series = ["page hits"];
							
							if(type === "-1")
						{
							page.chart.setSeries(series);
							page.chart.setPoints(points);
							page.chart.draw();
							
							updateOverview(overview);
							updateSegments(segments);
						}						
					}
				}
		});
});
