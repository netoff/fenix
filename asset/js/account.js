var account;
if(!account)
{
	account = {
		notify: function(message)
		{
			$("<div>", {"class": "notify", css: {position: "fixed", top:0, left: "50%", display: "none"}})
				.text(message).appendTo("body").show(0, function(){var width = $(this).width(); $(this).css({"margin-left": -width/2});})
				.delay(4000).fadeOut(2000, function(){$(this).remove();});
		},
		
		onPasswordChange: function(result)
		{
			var msg = "";
			
			if(result)
			{
				if(result.ok)
				{
					msg = "Password changed successfully";
				}
				else
				{
					msg = "Error trying to change password";
					if(result.msg === "do_not_match")
					{
						msg += ": Passwords do not match";
					}
					if(result.msg === "wrong_password")
					{
						msg += ": Wrong password!";
					}
					if(result.msg === "not_admin")
					{
						msg += ": You do not have right to do this!";
					}
				}
			}
			else
			{
				msg = "Error trying to change password. Please try again."
			}
			
			this.notify(msg);			
			
			$("#change-password-form input").removeAttr("disabled");
		},
		
		onAddWebsite: function(result)
		{
			var msg = "";
			
			if(result)
			{
				if(result.ok)
				{
					msg = "Website added successfully"
				}
				else
				{
					msg = "Error trying to add website"
					
					if(result.msg === "not_admin")
					{
						msg += ": You are not allowed to add website!";
					}
					if(result.msg === "error")
					{
						msg += ": Check your URL!";
					}
				}
			}
			else
			{
				msg = "Some error happened. Please try again.";
			}
			
			this.notify(msg);
			$("#add-website-form input").removeAttr("disabled");
		}
	};
}

function errorLabel(error, elem)
{
	elem.wrap($("<span style='position:relative;' />"));
	error.insertBefore(elem);
	error.css({top: - error.height() - 6});
}

$(function()
	{
		var panel = $("#account-settings"),
				tabs = $(".tabs a"), 
				selector = $("#js-tag-site-selector"),
				site_selector = $("#site-selector"),
				code = $("#js-tag-code"),
				cookie_name = "ssid";
		
		function setSiteSelected(id)
		{
			function setCookie(key, val, daysToExpire, path, domain, secure) 
			{
				var expiryDate;
				
				if (daysToExpire) 
				{
					expiryDate = new Date();
					expiryDate.setTime(expiryDate.getTime() + daysToExpire * 8.64e7);
				}
	
				document.cookie = key + '=' + encodeURIComponent(val) +
														(daysToExpire ? ';expires=' + expiryDate.toGMTString() : '') +
														';path=' + (path ? path : '/') +
														(domain ? ';domain=' + domain : '') +
														(secure ? ';secure' : '');
			}
			
			setCookie(cookie_name, id, 365);
		}
		
		function getSiteSelected()
		{
			function getCookie(key) 
			{
				var c_pattern = new RegExp('(^|;)[ ]*' + key + '=([^;]*)'),
						c = c_pattern.exec(document.cookie);
				return c ? decodeURIComponent(c[2]) : null;
			}
			
			return getCookie(cookie_name);			
		}
		
		function clearForms()
		{
			change_password_validator.resetForm();			
		}
		
		function generateCode(id)
		{
			var ret = "";
			
			ret += "<script src='http://cdn.kliknik.com/loader.js'></script>\n";
			ret += "<script type='text/javascript'>\n";
			ret += "_fnx_get_tracker('" + id + "');\n";
			ret += "</script>";
			
			return ret;
		}
		
		function loadSites()
		{
			var i, site, options = [], html, 
				id = null, 
				selected = getSiteSelected();
			
			if(_sites)
			{
				for(i = 0; i < _sites.length; i++)
				{
					site = _sites[i];
					
					if(site.id === selected) 
					{
						id = selected;
						options.push("<option selected='selected' value='");
					}
					else
					{
						options.push("<option value='");
					}
					
					options.push(site.id);options.push("'>");
						options.push(site.url);options.push("</option>");
				}
				
				id = id || _sites[0].id;
				
				_site_id = id;
				
				html = options.join("");
				
				site_selector.html(html);
				selector.html(html);
				
				code.val(generateCode(id));
			}
		}
		
		site_selector.change(function(){
				var id = $("option:selected", site_selector).val();
				_site_id = id;
				setSiteSelected(id);
				
				Reload();
		});
		
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
		loadSites();
		_updateTime();		
		
		change_password_validator = $("#change-password-form").validate({errorPlacement: errorLabel, 
				submitHandler: function(form) 
				{
					function disable_form()
					{
						$("#change-password-form input").attr("disabled", "disabled");
					};
					
					var d = $("#change-password-form").serialize();
					
					disable_form();
					
					$.ajax({
							url: "/analytics/account/change_password",
							cache: false,
							data: d,
							dataType: "script",
							type: "POST"
					});	
					
					return false;
				}
			});
		add_website_validator = $("#add-website-form").validate({errorPlacement: errorLabel, 
				submitHandler: function(form)
				{
					function disable_form()
					{
						$("#add-website-form input").attr("disabled", "disabled");
					}
						
					var d = $("#add-website-form").serialize();
					
					disable_form();
					
					$.ajax({
							url: "/analytics/account/add_website",
							cache: false,
							data: d,
							dataType: "script",
							type: "POST"
					});
					
					return false;
				}
		});
	});
