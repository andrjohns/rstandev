.onLoad <- function(libname, pkgname) {
  assign("stanc_context", V8::v8(), envir = topenv())
  stanc_context$source(system.file("stanc.js", package = "rstandev"))
}