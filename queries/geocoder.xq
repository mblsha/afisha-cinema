<geocoder>
<address>{doc($input)//GeocoderResponseMetaData/request/text()}</address>
{
for $f in doc($input)//featureMember
return
<object>
<country>{$f//CountryName/text()}</country>
<locality>{$f//LocalityName/text()}</locality>
<ll>{$f/GeoObject/Point/pos/text()}</ll>
</object>
}
</geocoder>
