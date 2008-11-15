<movies>
{
for $file in doc($input)//table[@class = "place"]/tr[child::td]
return
<movie>
<name>{fn:replace(fn:string-join($file/td/a[1]/text(), ' '), '\n', ' ')}</name>
<id>{fn:replace($file/td/a[1]/@href, '.*(?:&amp;|\?)event_id=(\d+).*', '$1')}</id>
</movie>
}
</movies>
