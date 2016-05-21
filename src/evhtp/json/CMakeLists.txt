if (NOT EVHTP_DISABLE_JSON)
	add_library(evhtp_json ${EVHTP_LIBTYPE} evhtp_json.c)

	install (FILES
		evhtp_json.h
		DESTINATION ${INCLUDE_INSTALL_DIR}/evhtp/json)
endif()
