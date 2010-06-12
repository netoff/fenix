var ___FNX_fenix_tracker_;
if(!___FNX_fenix_tracker_)
{
	___FNX_fenix_tracker_ = (function ()
		{
			var endpoint = "http://log.kliknik.com/lg/view",
					id,
					escapeMe = window.encodeURIComponent || escape,
					unescapeMe = window.decodeURIComponent || unescape;
					
			//private functions
			function setID(_id)
			{
				this.id = _id;
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

				document.cookie = name + '=' + escapeMe(val) +
					                  (daysToExpire ? ';expires=' + expiryDate.toGMTString() : '') +
					                  ';path=' + (path ? path : '/') +
					                  (domain ? ';domain=' + domain : '') +
					                  (secure ? ';secure' : '');
			}
			
			function getCookie(key) 
			{
				var c_pattern = new RegExp('(^|;)[ ]*' + key + '=([^;]*)'),
						c = c_pattern.exec(document.cookie);

				return c ? unescapeWrapper(c[2]) : 0;
			}
			function pingServer(url)
			{
				if(true)
					{console.log("URL: "+url);}
				else{
				var img = new Image(1,1);
				img.onload = function(){};
				img.src = url;}
			}
			function fenixLog(data)
			{
				var a = endpoint + "?_id=" + escapeMe(this.id),
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
					console.log("in track view");
					var url = document.location.href, 
							title = document.title,
							referrer = getReferrer();
					
					fenixLog({_e: "hit", _u: url, _t: title, _r: referrer});
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
					console.log("in proc");
					var i, queue, q;
							
					if(___FNX_fenix_queue_)
					{
						console.log("queue defined");
						queue = ___FNX_fenix_queue_;
						for(i = 0; i < queue.length; i ++)
						{
							console.log("queue has elems");
							q = queue[i];
							if(q.length > 0)
							{
								setID(q[0]);
								console.log("setting id");
								if(q.length > 1 && typeof q[1] === "function")
								{
									console.log("calling callback");
									q[1](this);
								}
								else
								{
									console.log("calling callback(default)");
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
