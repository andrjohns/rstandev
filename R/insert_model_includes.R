insert_includes <- function(model_code, include_paths) {
  if (grepl("#include", model_code)) {
    if (is.null(include_paths)) {
      stop("Stan code contains #include statements but no include ",
          "directories were provided!", call. = FALSE)
    }

    modlines <- strsplit(model_code, split = "\n")[[1]]
    include_lines <- grep("#include", modlines)
    includes <- modlines[include_lines]
    files <- gsub("#include\\s", "", includes)

    files_present <- lapply(include_paths, function(path) {
      list.files(file.path(path), full.names = TRUE)
    })

    include_code <- purrr::map_chr(files, function(file) {
      path <- purrr::map_chr(files_present, function(file_list) {
        res <- grep(files, file_list, value = TRUE)
        ifelse(length(res) == 0, "", res)
      })
      path <- path[(path != "")][1]
      if (is.na(path)) {
        stop("File ", file, " not found in specified include paths: ",
            paste(include_paths, collapse = ","), call. = FALSE)
      }
      readr::read_file(path)
    })
    modlines[include_lines] <- include_code
    model_code <- paste(modlines, collapse = "\n")
  }
  model_code
}
