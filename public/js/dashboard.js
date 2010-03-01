/*global $ */

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

function Chart()
{
	this.chartData = [];
	
	this.maxVal = 10;
	this.numPoints = 60;
	
	this.resolution = ""; //sec, min....
	
	this.maxValues = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 150, 200, 250, 300, 400, 500, 600, 700, 800, 900, 1000];
}

Chart.prototype.updatePoints = function (points)
{
	var i, a;
	
	for (i = 0; i < points.length; i++)
	{
		this.chartData.push(points[i]);
		if (this.chartData.length > this.numPoints + 1) 
		{
			this.chartData.shift();
		}
	}	
};

Chart.prototype.findMax = function (x, max)
{
	var i, maxI;
	
	if (x <= max) 
	{
		return max;
	}
	
	for (i = 0; i < this.maxValues.length; i++)
	{
		maxI = this.maxValues[i];
		
		if (x <= maxI) 
		{
			return maxI;
		}
	}
	
	return this.maxValues[this.maxValues.length - 1];
};

Chart.prototype.getDataPoints = function (it)
{
	var i, a, diff, points = [];
	
	diff = this.numPoints - this.chartData.length;
	
	for (i = 0; i < this.numPoints; i++)
	{
		if (i >= diff)
		{
			a = [i, this.chartData[i - diff][it]];
		}
		else
		{
			a = [i, 0];
		}
			
		points.push(a);
	}	
	
	return points;
};
Chart.prototype.setCanvas = function (canvas) 
{ 
	this.canvas = canvas; 
};
Chart.prototype.setSettings = function (settings) 
{ 
	this.settings = settings; 
};
Chart.prototype.draw = function ()
{
	
	$.plot(this.canvas, [
		{ data: this.getDataPoints(1), bars: { show: true, barWidth: 0.7, align: "center" }, label: "new visitors/" + this.resolution, stack: true },
		{ data: this.getDataPoints(2), bars: { show: true, barWidth: 0.7, align: "center" }, label: "returning visitors/" + this.resolution, stack: true},
		{ data: this.getDataPoints(0), lines: { show: true, steps: true }, points: { show: true, radius: 1}, label: "page hits/" + this.resolution }], this.settings);

	
};

var dashboard;
if (!dashboard) 
{
	dashboard = (function ()
	{
		return {
			site_id: "",
			
			setSiteId: function (id)
			{
				this.site_id = id;
			},
			
			timestamp: 0,
			timestampMin: 0,
	
			init: function ()
			{
				this.chart.setCanvas($('#trafic-live-chart'));
				this.chart.resolution = "sec";
				this.chart.settings = { 
					legend: { noColumns: 3, container: $('#chart-legend')},
					colors: ["#FF9900", "#7297BA", "#6f3"],
					yaxis: { min: 0, tickDecimals: 0},
					xaxis: { ticks: [[0, "1 min ago"], [9, ""], [19, ""], [29, "30 secs ago"], [39, ""], [49, ""], [59, "1 sec ago"]]},
					grid: { borderWidth: 0, tickColor: "#ccc", backgroundColor: {colors: ["#fff", "#ddd"]}}
				};
				
				this.chart.draw();
		
				this.chartMin.setCanvas($('#trafic-live-chart1'));
				this.chartMin.resolution = "min";
				this.chartMin.settings = { 
					legend: { noColumns: 3, container: $('#chart1-legend')},
					colors: ["#FF9900", "#7297BA", "#6f3"],
					yaxis: { min: 0, tickDecimals: 0},
					xaxis: { ticks: [[0, "1 h ago"], [9, ""], [19, ""], [29, "30 mins ago"], [39, ""], [49, ""], [59, "1 min ago"]]},
					grid: {borderWidth: 0, tickColor: "#ccc", backgroundColor: {colors: ["#fff", "#ddd"]}}
				};				
				
				this.chartMin.draw();
				
				this.adjustLayout();				
			},
	
			sameHeight: function (sections)
			{
				var max, i, j, section, max1;
				
				max = 0;
		
				for (i = 0; i < sections.length; i++)
				{
					//max = 0;
					
					section = $("div.window", sections[i]);
			
					for (j = 0; j < section.length; j++)
					{
						if ($(section[j]).height() > max)
						{
							max = $(section[j]).height();
						}					
					}
					
					for (j = 0; j < section.length; j++)
					{
						$(section[j]).height(max);
					}
					
					/*$(section).height(max);
					
					if(max > max1)
					{
						max1 = max;
					}*/
				}
				
				return max;
			},
			
			adjustLayout: function()
			{
				var max = this.sameHeight($("div.same-height"));
				
				$("#page-win").height(max + 20);
			},
	
			update: function () 
			{
				$.getScript('/app/stats/views?id=' + this.site_id + '&t=' + this.timestamp + '&t1=' + this.timestampMin);
			},
	
			chart: new Chart(),
			chartMin: new Chart(),
			
			updateData: function ()
			{
				var points, a, l;
				
				points = this.chart.chartData;
				l = points.length;
				
				a = points[l - 1];
				
				$("#stat-hps").html(a[0]);
				$("#stat-nvps").html(a[1]);
				$("#stat-rvps").html(a[2]);
			},
			
			updateDataMin: function ()
			{
				var points, a, l;
				
				points = this.chartMin.chartData;
				l = points.length;
				
				a = points[l - 1];
				
				$("#stat-hpm").html(a[0]);
				$("#stat-nvpm").html(a[1]);
				$("#stat-rvpm").html(a[2]);
			},
		
			//private
			lookupCountry: function (code)
			{
				return countries[code];
			},
			
			shortenText: function (text)
			{
				if(text.length > 29)
				{
					return text.substring(0, 26) + "...";
				}
				else
				{
					return text;
				}
			},
			
			makeList: function (list, x, y)
			{
				var i, a = [], html;
				
				a.push("<div class='stat-sep'></div>");
				
				for(i = 0; i < list.length; i++)
				{
					a.push("<div class='stat-label'><span class='title'>");
					if(x)
					{
						a.push(x(list[i][0]));
					}
					else
					{
						a.push(list[i][0]);
					}
					a.push("</span><span class='counter'>");
					if(y)
					{
						a.push(y(list[i][1]));
					}
					else
					{
						a.push(list[i][1]);
					}
					a.push("</span></div>");
				}
				
				html = a.join("");
				
				return html;
			},
			
			updateCountriesList: function (countries)
			{
				var html = this.makeList(countries, this.lookupCountry);
				
				$("#countries-list").html(html);
			},
			
			updatePagesList: function (pages)
			{
				var html = this.makeList(pages, this.shortenText);
				
				$("#pages-list").html(html);
			},
			
			updateReferrersList: function (referrers)
			{
				var html = this.makeList(referrers, this.shortenText);
				
				$("#referrers-list").html(html);
			},
			
			updateQueriesList: function (queries)
			{
				var html = this.makeList(queries, this.shortenText);
				
				$("#queries-list").html(html);
			},
			
			updateClock: function (new_time)
			{
				$("#time").text(new_time);
			}

		};
	}());
}

$(function ()
{
	dashboard.setSiteId(_site_id);
	dashboard.init();

	//Start the loop
	dashboard.update();	
});