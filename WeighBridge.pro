TEMPLATE = subdirs

SUBDIRS += qwt\
	Report \
    core \
    zgu100sgy \
    wbapp \
    common \
    ReportDesigner

!ordered {
        Report.depends = common
        core.depends = common qwt Report
        zgu100sgy.depends = common core
        weighbridge.depends = common core zgu100sgy
        ReportDesigner.depends = core Report
}

CODECFORTR  = Utf8
CODECFORSRC = Utf8
TRANSLATIONS = wb_en.ts
