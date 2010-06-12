$(function()
	{
		var panel = $("#account-settings"),
				tabs = $(".tabs a"), selector = $("#js-tag-site-selector"),
				code = $("#js-tag-code");
				
		function clearForms()
		{
		}
		function generateCode(id)
		{
			var ret = ""
			
			ret += "<script src='http://cdn.kliknik.com/loader.js'></script>\n";
			ret += "<script type='text/javascript'>\n";
			ret += "_fnx_get_tracker('" + id + "');\n";
			ret += "</script>";
			
			return ret;
		}
		function loadSites()
		{
			var i, sitem, options = [], id;
			
			if(_sites)
			{
				for(i = 0; i < _sites.length; i++)
				{
					site = _sites[i];
					
					if(i === 0) 
					{
						options.push("<option selected='selected' value='");
					}
					else
					{
						options.push("<option value='");
					}
					options.push(site.id);options.push("'>");
						options.push(site.url);options.push("</option>");
				}
				
				selector.html(options.join(""));
				
				id = $("option:selected", selector).val();
				if(id)
				{
					code.val(generateCode(id));
				}
			}
		}
		function _updateTime()
		{
			function loop()
			{
				setTimeout("updateTime()", 30000);
			}
			
			$.ajax({
					type: "GET",
					url: "/analytics/poll/account/time",
					dataType: "script",
					data: {
						id: _site_id
					},
					complete: loop,
					cache: false
			});			
		}
		
		selector.change(function()
			{
				id = $("option:selected", selector).val();
				
				if(id)
				{
					code.val(generateCode(id));
				}
			});
		tabs.click(function()
			{
				var tabnum = $(this).attr("tabnum");
				
				$("div.tab:visible").hide();
				$("div.tab[tabnum='" + tabnum + "']").fadeIn();
				return false;
			});
		
		$(".account-link").live("click", function()
			{
				
				panel.show();
				
				$("div.tab").hide();
				$("div.tab[tabnum='1']").show();
				
				$("div.alert", panel).fadeIn("slow");
				
				return false;
		});
		
		$(".alert a.close-link").click(function()
			{
				panel.hide();
				clearForms();
				
				return false;
			});
		
		updateTime = _updateTime;
		_updateTime();
		loadSites();
	});
