<movie detailed="true">
<!--doesn't work in &page=1 urls <name>{doc($input)//div[@class = 'announce']/h2/text()}</name>-->
<name>{fn:replace(doc($input)//h2/a/text(), '\n', ' ')}</name>
<id>{fn:replace(doc($input)//table[@class = 'chooser_city']/tr/td/a/@href, '.*(?:&amp;|\?)event_id=(\d+).*', '$1')}</id>
{
for $file in doc($input)//table[@class = 'chooser_data']/tbody/tr[child::td]
return
<cinema
name="{fn:replace(fn:string-join($file/td[1]/a/text(), ' '), '\n', ' ')}"
id="{fn:replace($file/td[1]/a[1]/@href, '.*(?:&amp;|\?)place_id=(\d+).*', '$1')}"
>
{
for $time in $file/td[3]//li[fn:not(child::a)]
return
<time>{$time/text()}</time>
}
{
for $time in $file/td[3]//li[child::a]
return
<time>{$time/a/i/text()}</time>
}
</cinema>
}
</movie>
