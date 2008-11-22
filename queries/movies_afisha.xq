<movies>
{
for $f in doc($input)//div[@class = 'b-theme-schedule']/div[@class = 'object']
return
<movie detailed="true">
<name>{fn:replace(fn:string-join($f//h3/a/text(), ' '), '\n', ' ')}</name>
<id>{fn:replace($f//h3/a/@href, '.*/movie/(\d+)/.*', '$1')}</id>
{
for $c in $f//table/tr
return
<cinema
name="{fn:replace(fn:string-join($c/td[1]/a/text(), ' '), '\n', ' ')}"
id="{fn:replace($c/td[1]/a/@href, '.*/cinema/(\d+)/.*', '$1')}"
>
{
for $t in $c//span[fn:not(child::a)]
return
<time>{$t/text()}</time>
}
{
for $t in $c//span[child::a]
return
<time reservation="{$t/a/@href}">{fn:replace(fn:string-join($t/a/text(), ''), '\n', '')}</time>
}
</cinema>
}
</movie>
}
</movies>
