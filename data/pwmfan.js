$('document').ready(function(){
	var temp_gauge = new RadialGauge({
		renderTo: 'temp_gauge',
		width: "250",
		height: "250",
		units: "(Temp) °C",
		minValue: 10,
		startAngle: 90,
		ticksAngle: 180,
		valueBox: true,
		maxValue: 60,
		majorTicks: [
			"10",
			"15",
			"20",
			"25",
			"30",
			"35",
			"40",
			"45",
			"50",
			"55",
			"60"
		],
		minorTicks: 5,
		strokeTicks: true,
		highlights: [
			{
				"from": 10,
				"to": 40,
				"color": "rgba(0, 84, 168, .2)"
			},
			{
				"from": 40,
				"to": 50,
				"color": "rgba(255, 222, 10, .75)"
			},
			{
				"from": 50,
				"to": 60,
				"color": "rgba(200, 50, 50, .75)"
			}
			
		],
		colorPlate: "#fff",
		borderShadowWidth: 0,
		borders: false,
		needleType: "arrow",
		needleWidth: 2,
		needleCircleSize: 7,
		needleCircleOuter: true,
		needleCircleInner: false,
		animationDuration: 1500,
		animationRule: "linear"
	}).draw();
	
	var fan_gauge = new RadialGauge({
		renderTo: 'fan_gauge',
		width: "250",
		height: "250",
		units: "(Fan) %",
		minValue: 0,
		startAngle: 90,
		ticksAngle: 180,
		valueBox: true,
		maxValue: 100,
		majorTicks: [
			"0",
			"10",
			"20",
			"30",
			"40",
			"50",
			"60",
			"70",
			"80",
			"90",
			"100"
		],
		minorTicks: 2,
		strokeTicks: true,
		highlights: [
			{
				"from": 0,
				"to": 40,
				"color": "rgba(27, 179, 3, .2)"
			},
			{
				"from": 40,
				"to": 80,
				"color": "rgba(255, 222, 10, .75)"
			},
			{
				"from": 80,
				"to": 100,
				"color": "rgba(200, 50, 50, .75)"
			}
			
		],
		colorPlate: "#fff",
		borderShadowWidth: 0,
		borders: false,
		needleType: "arrow",
		needleWidth: 2,
		needleCircleSize: 7,
		needleCircleOuter: true,
		needleCircleInner: false,
		animationDuration: 1500,
		animationRule: "linear",
	}).draw();
	function getTemperature(){
		$.ajax({
				type: "POST",
				dataType: 'json',
				url: "api/status",
				async: true,
				contentType: "application/json; charset=utf-8",
				success: function (msg) {
					console.log(msg);                
					var ct = document.getElementById('current_t');
					var ch = document.getElementById('current_h');
					var cfs = document.getElementById('current_fs');
					$(ct).html(msg.current_temperature);
					$(ch).html(msg.current_humidity);
					$(cfs).html(msg.current_fanspeed);
					temp_gauge.value = msg.current_temperature;
					fan_gauge.value = msg.current_fanspeed;
				},
				complete: function(){
					setTimeout(function(){getTemperature();}, 10000);
				},
				error: function(err){
					console.log(err);                
					var ct = document.getElementById('current_t');
					var ch = document.getElementById('current_h');
					var cfs = document.getElementById('current_fs');
					$(ct).html("n.d.");
					$(ch).html("n.d.");
					$(cfs).html("n.d.");
				}
		});
	}
	
 	getTemperature();
 	
 	$.ajax({
            type: "POST",
            dataType: 'json',
            url: "api/config",
            headers: {
                    'Access-Control-Allow-Origin': '*'
                },
            async: true,
            contentType: "application/json; charset=utf-8",
            success: function (msg) {
            	console.log(msg);
            	var content = '<table class="table">'
            	content += '<tr><th>Temperature (°C)</th><th>Speed (%)</th></tr>';
				for(i=0; i<msg.fan_speed_config.length; i++){
				    var config = msg.fan_speed_config[i];
					content += '<tr><td>' + config.temp + ' °C</td><td>' + config.speed + '%</td></tr>';
				}
				content += "</table>"

				$('#config-table').append(content);           
            	
            },
            error: function(err){
            	console.log(err);
            }
 	});

});

