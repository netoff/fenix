var ___FNX_fenix_tracker_;
if(!___FNX_fenix_tracker_)
{
	___FNX_fenix_tracker_ = (function ()
		{
			var endpoint = "http://log.kliknik.com/lg/js",
					ts_cookie = "___fnx_ts",
					orig_cookie = "___fnx_orig",
					id = "",
					escapeMe = window.encodeURIComponent || escape,
					unescapeMe = window.decodeURIComponent || unescape;
					
			//private functions
			function setID(_id)
			{
				id = _id;
			}
			function setCookie(key, val, daysToExpire, path, domain, secure) 
			{
				var expiryDate;
				
				if (daysToExpire) 
				{
					// time is in milliseconds
					expiryDate = new Date();
					// there are 1000 * 60 * 60 * 24 milliseconds in a day (i.e., 86400000 or 8.64e7)
					expiryDate.setTime(expiryDate.getTime() + daysToExpire * 8.64e7);
				}

				document.cookie = key + '=' + escapeMe(val) +
					                  (daysToExpire ? ';expires=' + expiryDate.toGMTString() : '') +
					                  ';path=' + (path ? path : '/') +
					                  (domain ? ';domain=' + domain : '') +
					                  (secure ? ';secure' : '');
			}
			
			function getCookie(key) 
			{
				var c_pattern = new RegExp('(^|;)[ ]*' + key + '=([^;]*)'),
						c = c_pattern.exec(document.cookie);

				return c ? unescapeMe(c[2]) : null;
			}
			function pingServer(url)
			{
				url = url + "&_=" + Math.random();
				var img = new Image(1,1);
				img.onload = function(){};
				img.src = url;
			}
			function fenixLog(data)
			{
				var a = endpoint + "?_id=" + escapeMe(id),
						k, v;
				if(data)
				{
					for(k in data)
					{
						if(data.hasOwnProperty(k))
						{
							v = data[k];
							a = a + "&" + escapeMe(k) + "=" + escapeMe(v);
						}
					}
				}
				pingServer(a);
			}
			
			function getReferrer()
			{
				var referrer = '';
				try 
				{
					referrer = top.document.referrer;
				} 
				catch (e) 
				{
					if (parent)
					{
						try
						{
							referrer = parent.document.referrer;
						} 
						catch (e2)
						{
							referrer = '';
						}
					}
				}
				if (referrer === '') 
				{
					referrer = document.referrer;
				}

				return referrer;
			}

			return {
				trackView: function (options)
				{
					var url = document.location.href, 
							title = document.title,
							referrer = getReferrer(),
							tm = -1, orig = "", t, o, now = new Date();
					t = parseInt(getCookie(ts_cookie), 10);
					if(t)
					{
						tm = parseInt(now.getTime()/1000 - t, 10);
						o = getCookie(orig_cookie);
						if(o)
						{
							orig = o;
						}
					}
					else
					{
						orig = referrer;
						if(referrer !== "")
						{
							setCookie(orig_cookie, referrer, 365);
						}
						else
						{
							setCookie(orig_cookie, "none", 365);
						}
					}
					setCookie(ts_cookie, parseInt(now.getTime()/1000, 10), 180);
					
					fenixLog({_e: "hit", _u: url, _t: title, _r: referrer, _o: orig, _tm: tm});
				},
				trackClick: function (options)
				{
				},
				trackEvent: function (options)
				{
				},
				trackGoal: function (options)
				{
				},
				proc: function ()
				{
					var i, queue, q;
							
					if(___FNX_fenix_queue_)
					{
						queue = ___FNX_fenix_queue_;
						for(i = 0; i < queue.length; i ++)
						{
							q = queue[i];
							if(q.length > 0)
							{
								setID(q[0]);
								if(q.length > 1 && typeof q[1] === "function")
								{
									q[1](this);
								}
								else
								{
									this.trackView();
								}
							}							
						}
					}
				}
			};
		}());
}
___FNX_fenix_tracker_.proc();
