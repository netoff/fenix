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
				var img = new Image(1,1);
				img.onload = function(){};
				img.src = url;
			}
			function logLog(data)
			{
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
					var url, title;
					
					fenixLog({});
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
					var i, ___FNX_fenix_queue_,
							queue, q, id;
							
					if(___FNX_fenix_queue_)
					{
						queue = ___FNX_fenix_queue_;
						for(i = 0; i < queue.lenght; i ++)
						{
							q = queue[i];
							if(q.length > 0)
							{
								setID(q[0]);
							}
							if(q.length > 1)
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
			};
		}());
}
