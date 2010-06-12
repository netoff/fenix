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
					if(segments && segments.length > 0)
					{
						var pages = segments.pages,
								len = pages.length, p, a;
								
						page.updateSegmentedList(pages_list_by_url, ["#", "Url", "Hits"], len, 
							function(i){
								p = pages[i];
								
								a = [];
								a.push(i + 1);a.push(p.name);a.push(p.hits);
									
								return a;
						});
					}
					else
					{
						pages_list_by_url.html("<p>There is no any data yet. Please <a href=\"#\" class=\"account-link\">install</a> javascript code so you can start collecting your data.</p>");
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
