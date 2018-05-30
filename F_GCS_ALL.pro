
TEMPLATE = subdirs

SUBDIRS  += $$PWD/F-GCS001/opmap/core/core.pro \
            $$PWD/F-GCS001/opmap/internals/internals.pro \
            $$PWD/F-GCS001/opmap/mapwidget/mapwidget.pro \
            $$PWD/F-GCS001/F-GCS001.pro

CONFIG += ordered

