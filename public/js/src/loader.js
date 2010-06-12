var _fnx_get_tracker;
if(!_fnx_get_tracker)
{
	_fnx_get_tracker = function(id, callback)
	{
		var ___FNX_fenix_tracker_,
				___FNX_fenix_queue_; 
		___FNX_fenix_queue_ = ___FNX_fenix_queue_ || [];
		
		___FNX_fenix_queue_.push([id, callback]);
		
		if(!___FNX_fenix_tracker_)
		{
			//load script
			var sc = document.createElement("script");
			sc.async = true;
			sc.type = "text/javascript";
			sc.src = "http://asset.kliknik.my/fenix.js";
			
			var s = document.getElementsByTagName('script')[0]; 
			s.parentNode.insertBefore(sc, s);		
			
			return;
		}
		if(___FNX_fenix_tracker_.proc)
		{
			___FNX_fenix_tracker_.proc();
		}
	};
}