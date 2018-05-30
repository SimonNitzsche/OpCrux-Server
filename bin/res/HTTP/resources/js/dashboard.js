let Dashboard = {
    isLoggedIn: false,
    loadShow: function(text) {
        $("#loadingModal .modal-text").get(0).innerText=text;
        $("#loadingModal").modal({
            backdrop: 'static',
            keyboard: false,
            show: true
        });
    },
    loadHide: function() {
        $("#loadingModal").modal("hide");
    },
    loginShow: function() {
        if(this.isLoggedIn) return;
        $("#loginModal").modal({
            backdrop: 'static',
            keyboard: false,
            show: true
        });
    },
    loginHide: function() {
        $("#loginModal").modal("hide");
    },
    loginError: function(text) {
        $("#loginModal #errorText").get(0).innerText=text;
    },
    startup: function() {
        this.registerURLChangeHandler();
        this.loadShow("Testing authentification...");
        this.testLogin();
    },
    performLogin: function() {
        this.loginError(""); // reset error msg
        this.loginHide();
        this.loadShow("Logging in...");

        let usr = $("#loginModal #username").get(0).value;
        let pwd = $("#loginModal #password").get(0).value;
        let authBsc = btoa(usr + ':' + pwd);
        console.log(authBsc);
        $.ajax({
            url: "/auth",
            type: "post",
            headers: {
                Authorization: 'Basic ' + authBsc
            },
            complete: (data) => {
                this.loadHide();
                if(data.status==200) {
                    this.loginError("");
                    this.loginHide();
                    location.reload();
                } else {
                    this.loginError("["+data.status+"] "+data.responseText);
                    this.loginShow();
                }
            }
        });
    },
    loadData: function() {
        $.get("/api/"+this.getLocation(), (data) => {
            let j = JSON.parse(data);
            $("view").find("*").get().forEach((node)=>{
                for(let attr of node.attributes) {
                    const parseData = function(ni, ji) {
                        const regex = /\{([^\}]+)\}/g;
                        let m; while ((m = regex.exec(ni)) !== null) {
                            // This is necessary to avoid infinite loops with zero-width matches
                            if (m.index === regex.lastIndex) {
                                regex.lastIndex++;
                            }

                            // The result can be accessed through the `m`-variable.
                            const ajax_query = m[1].split(".");
                            let ajax_query_current = ji;
                            for(let i = 0; i<ajax_query.length; i++) {
                                ajax_query_current = ajax_query_current[ajax_query[i]];
                            }

                            ni = ni.replace(m[0],ajax_query_current);
                        }
                        return ni;
                    };
                    if(attr.nodeName.substr(0,"data-ajax-".length)=="data-ajax-"){
                        node.setAttribute(attr.nodeName.substr("data-ajax-".length), parseData(attr.nodeValue, j));
                        node.removeAttribute(attr.nodeName);
                    } else if(attr.nodeName.substr(0,"data-ajax".length)=="data-ajax") {
                        node.innerHTML = parseData(attr.nodeValue, j);
                        node.removeAttribute(attr.nodeName);
                    }
                }
            });

            this.checkListActiveSelection();
            this.loadHide();
        });
    },
    loadPageContent: function(here) {
        $("view").empty();
        console.log(here);
        switch(here.split("/")[0]) {
            case "master": {
                DashboardTemplates.view.setMaster();
                break;
            }
            case "mod": {
                switch (here.split("/")[1]) {
                    case "names": {
                        console.log("setModNames");
                        DashboardTemplates.view.setModNames();
                        break;
                    }
                    default: {
                        return;
                    }
                }
            }
            default: {
                return;
            }
        }
        this.loadData();
        this.checkListActiveSelection();
    },
    checkListActiveSelection: function() {
        $("li").each(function(i) {
            if($(this).data("toggle")!="collapse") {
                if ($(this).find("a").attr("href")=='#'+Dashboard.getLocation()) {
                    $(this).addClass("active");
                }
                else {
                    $(this).removeClass("active");
                }
            }
        });
    },
    uriChangeHandler: function() {
        $("view").empty();
        if(Dashboard.getLocation()=="login") {
            Dashboard.loginShow();
            return;
        }
        else if(Dashboard.getLocation()=="") {
            Dashboard.gotoPage("master");
            return;
        }
        Dashboard.loadPageContent(Dashboard.getLocation());
        Dashboard.checkListActiveSelection();
    },
    registerURLChangeHandler: function(){
        $(window).on('hashchange',this.uriChangeHandler);
        this.uriChangeHandler();
    },
    gotoPage: function(here) {
        location.hash = here;
    },
    getLocation: function() {
        if(location.hash==""||location.hash[0]!="#") {
            return location.hash;
        } else {
            return location.hash.substr(1);
        }
    },
    testLogin: function() {
        $.get("/api/isLoggedIn", (data) => {
            if(JSON.parse(data).isLoggedIn) {
                this.isLoggedIn=true;
                if(this.getLocation()=="login") {this.gotoPage("master");this.loginHide();}
                DashboardTemplates.navBar.insertAll();
                this.registerButtonHandler();
                this.checkListActiveSelection();
                this.uriChangeHandler();
                this.loadHide();
            } else {
                this.gotoPage("login");
                this.loadHide();
            }
        });
    },
    registerButtonHandler: function() {
        $("li").click(function (e) {
            if($(this).has("a")&&$(this).data("toggle")!="collapse") {
                Dashboard.gotoPage($(this).find("a").attr("href"));
            }
        });
    }
};

let DashboardTemplates = {
    navBar: {
        insertMaster: function() {
            this.append(DashboardTemplates.getTemplate("template-nav-item-master"));
        },
        insertDB: function() {
            this.append(DashboardTemplates.getTemplate("template-nav-item-db"));
        },
        insertAuth: function() {
            this.append(DashboardTemplates.getTemplate("template-nav-item-auth"));
        },
        insertWorld: function() {
            this.append(DashboardTemplates.getTemplate("template-nav-item-world"));
        },
        insertModeration: function() {
            this.append(DashboardTemplates.getTemplate("template-nav-item-moderation"));
        },
        insertAll: function() {
            this.insertMaster();
            this.insertDB();
            this.insertModeration();
            this.insertAuth();
            this.insertWorld();
        },
        append: function(itm) {
            $(".nav-side-menu #menu-content").append(itm);
        }
    },
    view: {
        setMaster: function() {
            return this.set(DashboardTemplates.getTemplate("template-view-master"));
        },
        setModNames: function() {
            return this.set(DashboardTemplates.getTemplate("template-view-mod-names"));
        },
        set: function(itm) {
            return $("view").append(itm);
        }
    },
    getTemplate: function(id) {
        return $("template#"+id).get(0).innerHTML;
    }
}

$(function() {Dashboard.startup();});
