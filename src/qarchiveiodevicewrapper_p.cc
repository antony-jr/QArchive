#include "qarchiveiodevicewrapper_p.hpp"

using namespace QArchive;

IODeviceWrapper::IODeviceWrapper(QObject *parent)
	: QObject(parent),
	  m_Ptr(nullptr) {  }

IODeviceWrapper::IODeviceWrapper(QIODevice *ptr, QObject *parent)
	: IODeviceWrapper(parent),
	  m_Ptr(ptr) { }

IODeviceWrapper::~IODeviceWrapper() { 
	if(m_Ptr) {
		m_Ptr->deleteLater();
	}
}

QIODevice *IODeviceWrapper::take() {
	QIODevice *ptr = m_Ptr;
	m_Ptr = nullptr;
	return ptr;
}
