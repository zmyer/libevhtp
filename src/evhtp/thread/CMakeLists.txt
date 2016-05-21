if (NOT EVHTP_DISABLE_EVTHR)
	set(LIBEVHTP_SOURCES "${LIBEVHTP_SOURCES} evhtp/evhtp_thread.c")

	install(FILES
		evhtp_thread.h
		DESTINATION ${INCLUDE_INSTALL_DIR}/evhtp/thread)
endif()
