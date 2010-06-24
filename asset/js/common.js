var countries = { "--": "N/A", "AP": "Asia/Pacific Region","EU": "Europe","AD":"Andorra","AE":"United Arab Emirates",
	  "AF": "Afghanistan","AG":"Antigua and Barbuda","AI":"Anguilla","AL":"Albania","AM":"Armenia","AN":"Netherlands Antilles",
	  "AO":"Angola","AQ":"Antarctica","AR":"Argentina","AS":"American Samoa","AT":"Austria","AU":"Australia","AW":"Aruba",
	  "AZ":"Azerbaijan","BA":"Bosnia and Herzegovina","BB":"Barbados",
	  "BD":"Bangladesh","BE":"Belgium","BF":"Burkina Faso","BG":"Bulgaria","BH":"Bahrain","BI":"Burundi","BJ":"Benin",
	  "BM":"Bermuda","BN":"Brunei Darussalam","BO":"Bolivia",
	  "BR":"Brazil","BS":"Bahamas","BT":"Bhutan","BV":"Bouvet Island","BW":"Botswana","BY":"Belarus","BZ":"Belize",
	  "CA":"Canada","CC":"Cocos (Keeling) Islands","CD":"Congo, The Democratic Republic of the",
	  "CF":"Central African Republic","CG":"Congo","CH":"Switzerland","CI":"Cote D'Ivoire","CK":"Cook Islands",
	  "CL":"Chile","CM":"Cameroon","CN":"China","CO":"Colombia","CR":"Costa Rica",
	  "CU":"Cuba","CV":"Cape Verde","CX":"Christmas Island","CY":"Cyprus","CZ":"Czech Republic","DE":"Germany","DJ":"Djibouti",
	  "DK":"Denmark","DM":"Dominica","DO":"Dominican Republic",
	  "DZ":"Algeria","EC":"Ecuador","EE":"Estonia","EG":"Egypt","EH":"Western Sahara","ER":"Eritrea","ES":"Spain","ET":"Ethiopia",
	  "FI":"Finland","FJ":"Fiji",
	  "FK":"Falkland Islands (Malvinas)","FM":"Micronesia, Federated States of","FO":"Faroe Islands","FR":"France","FX":"France, Metropolitan",
	  "GA":"Gabon","GB":"United Kingdom","GD":"Grenada","GE":"Georgia","GF":"French Guiana",
	  "GH":"Ghana","GI":"Gibraltar","GL":"Greenland","GM":"Gambia","GN":"Guinea","GP":"Guadeloupe","GQ":"Equatorial Guinea",
	  "GR":"Greece","GS":"South Georgia and the South Sandwich Islands","GT":"Guatemala",
	  "GU":"Guam","GW":"Guinea-Bissau","GY":"Guyana","HK":"Hong Kong","HM":"Heard Island and McDonald Islands","HN":"Honduras",
	  "HR":"Croatia","HT":"Haiti","HU":"Hungary","ID":"Indonesia",
	  "IE":"Ireland","IL":"Israel","IN":"India","IO":"British Indian Ocean Territory","IQ":"Iraq","IR":"Iran, Islamic Republic of",
	  "IS":"Iceland","IT":"Italy","JM":"Jamaica","JO":"Jordan",
	  "JP":"Japan","KE":"Kenya","KG":"Kyrgyzstan","KH":"Cambodia","KI":"Kiribati","KM":"Comoros","KN":"Saint Kitts and Nevis",
	  "KP":"Korea, Democratic People's Republic of","KR":"Korea, Republic of","KW":"Kuwait",
	  "KY":"Cayman Islands","KZ":"Kazakhstan","LA":"Lao People's Democratic Republic","LB":"Lebanon","LC":"Saint Lucia",
	  "LI":"Liechtenstein","LK":"Sri Lanka","LR":"Liberia","LS":"Lesotho","LT":"Lithuania",
	  "LU":"Luxembourg","LV":"Latvia","LY":"Libyan Arab Jamahiriya","MA":"Morocco","MC":"Monaco","MD":"Moldova, Republic of",
	  "MG":"Madagascar","MH":"Marshall Islands","MK":"Macedonia","ML":"Mali",
	  "MM":"Myanmar","MN":"Mongolia","MO":"Macau","MP":"Northern Mariana Islands","MQ":"Martinique","MR":"Mauritania",
	  "MS":"Montserrat","MT":"Malta","MU":"Mauritius","MV":"Maldives",
	  "MW":"Malawi","MX":"Mexico","MY":"Malaysia","MZ":"Mozambique","NA":"Namibia","NC":"New Caledonia","NE":"Niger",
	  "NF":"Norfolk Island","NG":"Nigeria","NI":"Nicaragua",
	  "NL":"Netherlands","NO":"Norway","NP":"Nepal","NR":"Nauru","NU":"Niue","NZ":"New Zealand","OM":"Oman","PA":"Panama","PE":"Peru",
	  "PF":"French Polynesia",
	  "PG":"Papua New Guinea","PH":"Philippines","PK":"Pakistan","PL":"Poland","PM":"Saint Pierre and Miquelon",
	  "PN":"Pitcairn Islands","PR":"Puerto Rico","PS":"Palestinian Territory","PT":"Portugal","PW":"Palau",
	  "PY":"Paraguay","QA":"Qatar","RE":"Reunion","RO":"Romania","RU":"Russian Federation","RW":"Rwanda","SA":"Saudi Arabia",
	  "SB":"Solomon Islands","SC":"Seychelles","SD":"Sudan",
	  "SE":"Sweden","SG":"Singapore","SH":"Saint Helena","SI":"Slovenia","SJ":"Svalbard and Jan Mayen","SK":"Slovakia",
	  "SL":"Sierra Leone","SM":"San Marino","SN":"Senegal","SO":"Somalia",
	  "SR":"Suriname","ST":"Sao Tome and Principe","SV":"El Salvador","SY":"Syrian Arab Republic","SZ":"Swaziland",
	  "TC":"Turks and Caicos Islands","TD":"Chad","TF":"French Southern Territories","TG":"Togo","TH":"Thailand",
	  "TJ":"Tajikistan","TK":"Tokelau","TM":"Turkmenistan","TN":"Tunisia","TO":"Tonga","TL":"Timor-Leste","TR":"Turkey",
	  "TT":"Trinidad and Tobago","TV":"Tuvalu","TW":"Taiwan",
	  "TZ":"Tanzania, United Republic of","UA":"Ukraine","UG":"Uganda","UM":"United States Minor Outlying Islands",
	  "US":"United States","UY":"Uruguay","UZ":"Uzbekistan","VA":"Holy See (Vatican City State)","VC":"Saint Vincent and the Grenadines",
	  "VE":"Venezuela",
	  "VG":"Virgin Islands, British","VI":"Virgin Islands, U.S.","VN":"Vietnam","VU":"Vanuatu","WF":"Wallis and Futuna",
	  "WS":"Samoa","YE":"Yemen","YT":"Mayotte","RS":"Serbia","ZA":"South Africa","ZM":"Zambia","ME":"Montenegro",
  "ZW":"Zimbabwe","A1":"Anonymous Proxy","A2":"Satellite Provider","O1":"Other","AX":"Aland Islands","GG":"Guernsey",
  "IM":"Isle of Man","JE":"Jersey","BL": "Saint Barthelemy","MF":"Saint Martin"};
function lookupCountry(code)
{
	var ret = countries[code];
	if(ret)
	{
		return ret;
	}
	return code;
}
function formatInteger(i)
{
	return String(i).replace(/(\d)(?=(\d{3})+([.]|$))/g,"$1,");
}
function formatFloat(f)
{
	return String(f).replace(/(\d)(?=(\d{3})+([.]|$))/g,"$1,");
}
function formatPercent(p)
{
	return formatFloat(p) + "%";
}
var page;
function sameHeight()
{
	var i, j, regions = $("div.same-height");
	
	for(i = 0; i < regions.length; i++)
	{
		var windows = $("div.window", regions[i]);
		
		var max = 0;
		
		for(j = 0; j < windows.length; j++)
		{
			if(windows.eq(j).height() > max)
			{
				max = windows.eq(j).height();
			}
		}
		
		windows.height(max);
	}
}
var picker_html = "<form id=\"date-range-selector-form\">" +
									"<div>" +
									"<input type=\"submit\" class=\"selector-submit\" value=\"Refresh\"" + 
									" id=\"date-range-selector-submit\"/>"+
									"<input type=\"text\" name=\"range\" class=\"selector-input\""+
									" autocomplete=\"off\" title=\"Date range\" alt=\"Date range\"/>&nbsp;&nbsp;&nbsp;"+
									"</div><div class=\"date-range-predefined-selectors\">"+
									"<a class=\"a-last-30-days\" href=\"#\">Last 30 days</a> |"+ 
									"<a class=\"a-last-7-days\" href=\"#\">Last 7 days</a> |" +									
									"<a class=\"a-custom-range\" href=\"#\">Select custom range</a>"+
									"</div></form>";
function daterangePicker()
{
	$("#daterange-picker-placeholder").html(picker_html);
		
	var that = this;
	
	function formatDate(d)
	{
		return d.toString("MM/dd/yyyy");
	}
	
	function formatRange(d1, d2)
	{
		return formatDate(d1) + " - " + formatDate(d2);
	}
	
	function lastNDates(n)
	{
		var ret = [];
		
		var today = Date.today();
		var ago = (n).days().ago();
		
		ret[0] = ago;
		ret[1] = today;
		
		return ret;
	}
	
	function lastNDays(n)
	{
		var dates = lastNDates(n - 1);
		
		return formatRange(dates[0], dates[1]);
	}
	
	function last7Days() { return lastNDays(7); }
	function last30Days() { return lastNDays(30); }
	
	function getDatesFromString(s)
	{
		var ret = [];
		
		var dates = s.split("-");
		
		
		if(dates.length === 2)
		{
			ret[0] = Date.parse(dates[0]).setTimezoneOffset(0);
			ret[1] = Date.parse(dates[1]).setTimezoneOffset(0);
		}
		
		return ret;
	}
	
	var daterange_input = $("#date-range-selector-form .selector-input");
	
	var today = Date.today();
				
	var dates = lastNDates(30);
	
	daterange_input.val(lastNDays(30));
	
	daterange_input.DatePicker({
			mode: 'range',
			calendars: 2,
			format: 'm/d/Y',
			date: [dates[0], dates[1]],
			onBeforeShow: function()
			{
				var dates = getDatesFromString(daterange_input.val());
		
				if(dates.length)
				{
					daterange_input.DatePickerSetDate([dates[0], dates[1]], true);
				}
			},
			onChange: function (formated, dates)
			{						
				daterange_input.val(formated[0] + " - " + formated[1]);
			},
			onRender: function (date)
			{					
				var val = date.valueOf();
		
				if(val > today.valueOf())
				{
					return {'disabled': true, 'selected': false};
				}
				
				/*
				var selected = getDatesFromString(daterange_input.val());
				
				if(selected.length === 2)
				{
					var start = selected[0].valueOf();
					var end = selected[1].valueOf();						
					
					if(val >= start && val <= end)
					{
						return {'selected': true, 'disabled': false};
					}					
				}*/		
				return {'selected': false, 'disabled': false};
			}
	});
	
	return {
		get: function()
		{
			return daterange_input.val();
		},
		getDates: function()
		{
			return getDatesFromString(this.get());
		},
		set: function(val)
		{
			daterange_input.val(val);
		},
		show: function()
		{
			daterange_input.DatePickerShow();
		},
		setLast7: function()
		{
			this.set(last7Days());
		},
		setLast30: function()
		{
			this.set(last30Days());
		}
	};
}
if(!page)
{
	page = (function(){
			var that = this;		
			var report_chooser, daterange_picker;
			
			function getReportType()
			{
				var type = $("a.active", report_chooser).attr("report_type");
				return type;
			}
			
			function _init()
			{
				var report_chooser = $("#report-chooser"),
						report_title = $("#report-name");
						
				daterange_picker = daterangePicker();				
				
				$("#date-range-selector-form .selector-submit").click(function (){
						page.update();				
						return false;
				});
			
				$("#date-range-selector-form .a-custom-range").click(function (){
						daterange_picker.show();
				
						return false;				
				});
			
				$("#date-range-selector-form .a-last-7-days").click(function (){
						daterange_picker.setLast7();
						page.update();	
				
						return false;				
				});
			
				$("#date-range-selector-form .a-last-30-days").click(function (){						
						daterange_picker.setLast30();
						page.update();
				
						return false;
				});
			
				$("a", report_chooser).click(function (){
						$("a.active", that.report_chooser).removeClass("active");
						$(this).addClass("active");
						report_title.text($(this).text());
						page.update();
				
						return false;
				});
				
				page.chart.getDataPoints = function (it, n)
				{
					var i, points = [],
							dates = daterange_picker.getDates(),
							d = dates[0];
				
					if(it === 3 && n === 4)
					{
						for(i = 0; i < this.chartData.length; i++)
						{
							points.push([d.getTime(), this.chartData[i][0] + this.chartData[i][1] + this.chartData[i][2]]);
							d.add({'days': 1});
						}
					
						return points;
					}
					else
					{
						for(i = 0; i < this.chartData.length; i++)
						{		
							points.push([d.valueOf(), this.chartData[i][it]]);
							d.add({'days': 1});
						}
					
						return points;
					}
				
					return [];
				};
				
				page.chart.getPoints = function (it, n)
				{
					if((it === 3 && n === 4) || n === 1)
					{
						return { show: true, radius: 3};
					}
					
					return { show: false };
				};
				
				page.chart.getBars = function (it, n)
				{
					if((it === 3 && n === 4) || n === 1)
					{
						return {show: false};
					}
					else
					{
						return {show: true, barWidth: 1000 * 60 * 60 * 18, align: 'center'};
					}
				};
				
				page.chart.getLines = function (it, n)
				{	
					if((it === 3 && n === 4) || n === 1)
					{
						return {show: true, lineWidth: 3, color: "#7297BA"};
					}
						
					return {show: false};
				}
				
				page.chart.getStack = function (it, n)
				{
					if((it === 3 && n === 4) || n === 1)
					{
						return null;
					}
					
					return true;
				};
				page.chart.getColor = function (it, n)
				{
					var colors = ["#FF9900", "#6f3", "#eee", "#7297BA"];

					if(n === 1)
					{
						return colors[3];
					}
					else
					{
						return colors[it];
					}
				};
				
				page.chart.tooltipFormat = function (item, series)
				{				
					var label = series[item.seriesIndex];
					var i = item.series.data[item.dataIndex][1];
					var date = new Date(item.series.data[item.dataIndex][0]);
					
					return "<em>" + date.toString("MMM d, yyyy") + "</em><br />" + 
						"<strong>" + formatInteger(i) + "</strong>&nbsp;" + label;
				};
				
				page.chart.setSettings({
					legend: { noColumns: 4, container: $("#chart-legend")},
					colors: ["#FF9900", "#6f3", "#eee", "#7297BA"],
					yaxis: {min: 0, tickDecimals: 0},
					xaxis: {mode: 'time', minTickSize: [1, "day"]},
					grid: {hoverable: true, borderWidth: 1, borderColor: "#bbb", tickColor: "#ccc", backgroundColor: {colors: ["#fff", "#ddd"]}}
				});
				
				page.chart.setSeries([""]);
				
				page.chart.setCanvas($("#report-chart"));
				
				page.chart.draw();
			}
			
			
			return {
			//page init function
				init: function (init_foo)
				{
					_init();
					init_foo(this);
					page.setSite(_site_id);							
				
					sameHeight();
					page.update();
				},				
			
				chart: new Chart(30, "day"),
			
				site_id: "",
			
				update: function ()
				{
					//this.loaderOn();
					
					var date_range = daterange_picker.get();
					
					if(date_range)
					{
						var dates = date_range.split("-");
						
						if (dates.length === 2)
						{
							var start = dates[0],
									end = dates[1],
									type = getReportType();
								
							if(type)
							{
										$.ajax({
												type: "GET",
												url: this.endpoint,
												dataType: "script",
												data: {
													id: _site_id,
													t: type,
													sd: start,
													ed: end
												},
												cache: false
										});
							}
						}
						
					}
					
					//this.loaderOff();
				},
				
				changeSite: function (site_id)
				{
					this.site_id = site_id;
				},
				
				setSite: function (site_id)
				{
					this.site_id = site_id;
				},
				
				setEndpoint: function (ep)
				{
					this.endpoint = ep;
				},
				
				updateSegmentedList: function (elem, header, n, callback)
				{
					function makeSegmentedList()
					{
						var ret = [], i, j, a;
						
						ret.push("<table class='segmented-list'><thead><tr>");
						for(i = 0; i < header.length; i++)
						{
							if(i > 1)
							{
								ret.push("<th style=\"text-align:right;\">");
							}
							else
							{
								ret.push("<th>");
							}
							ret.push(header[i]);ret.push("</th>");
						}
						ret.push("</tr></thead><tbody>");
						
						for(i = 0; i < n; i++)
						{
							a = callback(i);
							
							if(i%2 != 0){ret.push("<tr class='alt'>");}else{ret.push("<tr>");};
							for(j = 0; j < a.length; j ++)
							{
								if(j > 1)
								{
									ret.push("<td style=\"text-align:right;\">");
								}
								else
								{
									ret.push("<td>");
								}
								ret.push(a[j]);ret.push("</td>");
							}
							ret.push("</tr>");
						}
						ret.push("</tbody></table>");
						
						return ret.join("");
					}
				
					if(elem && header && header.length > 0 && n > 0 && callback)
					{
						elem.html(makeSegmentedList());
						elem.find("table").tablePagination({
								optionsForRows: [10],
								rowsPerPage: 10,
								firstArrow : (new Image()).src="/img/first.gif",
								prevArrow : (new Image()).src="/img/prev.gif",
								lastArrow : (new Image()).src="/img/last.gif",
								nextArrow : (new Image()).src="/img/next.gif"
						});
						var h = elem.find("table").height();
						elem.parent(".window").height(h + 60);
						sameHeight();
					}
				}
				
			};
	}());
}

