var _fnx_get_tracker,
 ___FNX_fenix_queue_ = ___FNX_fenix_queue_ || [];
if(!_fnx_get_tracker)
{
	_fnx_get_tracker = function(id, callback)
	{
		var ___FNX_fenix_tracker_, proc;
		
		___FNX_fenix_queue_.push([id, callback]);
		
		if(!___FNX_fenix_tracker_)
		{
			//load script
			var sc = document.createElement("script");
			sc.async = true;
			sc.type = "text/javascript";
			sc.src = "http://cdn.kliknik.com/fenix.js";
			
			var s = document.getElementsByTagName('script')[0]; 
			s.parentNode.insertBefore(sc, s);		
			
			return;
		}
		proc = ___FNX_fenix_tracker_.proc;
		if(proc && typeof proc === 'function')
		{
			proc();
		}
	};
}
