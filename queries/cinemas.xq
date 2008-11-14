<cinemas>
{
for $file in doc($input)//table[@class = "place"]/tr[child::td]
return
<cinema>
<name>{fn:replace($file/td/a/text(), '\n', '')}</name>
<id>{fn:replace($file/td/a/@href, '.*(?:&amp;|\?)place_id=(\d+).*', '$1')}</id>
</cinema>
}
</cinemas>
