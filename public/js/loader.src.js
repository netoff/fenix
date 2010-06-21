var _FNX_getTracker;
if(!_FNX_getTracker)
{
	_FNX_getTracker = function(id, callback)
	{
		var ___FNX_fenix_tracker_,
				___FNX_fenix_queue_ = ___FNX_fenix_queue_ || [];
		
		___FNX_fenix_queue.push([id, callback]);
		
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
	}
}

/*sample installation

<script src="http://asset.kliknik.com/loader.js"></script>
<script type="text/javascript">
	_FNX_getTracker("abcddefgh");	
</script>

<script src="http://asset.kliknik.com/loader.js"></script>
<script type="text/javascript">
	_FNX_getTracker("abcddefgh", function(tracker){
		tracker.
	});	
</script>

*/
