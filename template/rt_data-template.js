{% if Project.TabView is defined and Project.TabView %}
function show_{{ Project.Name|lower }}_data(displayBody, dataObj)
{
    displayBody.find(".tab-content").each(function(index){
        ShowDeviceData(displayBody.find(".tab-content").eq(index), dataObj[displayBody.find(".tab-widget li").eq(index).text()]);
     }) 
    displayBody.find(".tab-widget li").off("click").on("click", function () {
        $(this).addClass("active");
        $(this).siblings().removeClass("active");
        var i = $(this).index();
        $(this).parent().parent().parent().find(".tab-content").eq(i).show();
        $(this).parent().parent().parent().find(".tab-content").eq(i).siblings(".tab-content").hide();
    })
   if(displayBody.find(".tab-widget li.active").length == 0){
     displayBody.find(".tab-widget li").eq(0).trigger("click")
   }
}
{% endif %}
