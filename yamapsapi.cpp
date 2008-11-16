#include "yamapsapi.h"

// мой ключик:
// ALA-HEkBAAAAWA7dRQMA1tU9drgY14m-mw5ZOdnrjLKyLIwAAAAAAAAAAACEbj_BEO_HXptK6butt69Bqhhowg==

// <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
// 	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
// 	<html xmlns="http://www.w3.org/1999/xhtml">
// 	<head>
// 		<meta http-equiv="Content-Type" content="text/html" />
// 		<script src="http://api-maps.yandex.ru/0.8/?key=
// 		ALA-HEkBAAAAWA7dRQMA1tU9drgY14m-mw5ZOdnrjLKyLIwAAAAAAAAAAACEbj_BEO_HXptK6butt69Bqhhowg=="
// 		type="text/javascript"></script>
// 
// 		<script type="text/javascript" charset="utf-8">
// 		 var map;
// 		 function init () {
// 		   map = new YMaps.Map( document.getElementById("YMapsID") );
// 		   map.setCenter(new YMaps.GeoPoint(37.64,55.76 ), 10, 
// 				  YMaps.MapType.MAP);
// 		 };
// 		</script>
// 	</head>
// 
// 	<body onload="init();">
// 		<div id="YMapsID" style="height:400px; width:600px;"></div>
// 	</body>
// 
// 	</html>

QString YaMapsApi::key()
{
	return "ALA-HEkBAAAAWA7dRQMA1tU9drgY14m-mw5ZOdnrjLKyLIwAAAAAAAAAAACEbj_BEO_HXptK6butt69Bqhhowg==";
}
