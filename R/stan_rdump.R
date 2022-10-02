data_list2array <- function(x) {
  len <- length(x)
  if (len == 0L)  return(NULL)

  dimx1 <- dim(x[[1]])

  if (any(sapply(x, function(xi) !is.numeric(xi))))
    stop("all elements of the list should be numeric")
  if (is.null(dimx1)) dimx1 <- length(x[[1]])
  lendimx1 <- length(dimx1)

  if (len > 1) {
    d <- sapply(x[-1],
                function(xi) {
                  dimxi <- dim(xi)
                  if (is.null(dimxi)) dimxi <- length(xi)
                  identical(dimxi, dimx1)
                })
    if (!all(d)) stop("the dimensions for all elements (array) of the list are not same")
  }

  # TODO(?): check if x is numeric or array.
  x <- do.call(c, x)
  dim(x) <- c(dimx1, len)
  aperm(x, c(lendimx1 + 1L, seq_len(lendimx1)))
}

stan_rdump <- function(list, quiet = TRUE) {
  l2 <- NULL
  addnlpat <- paste0("(.{1,", 100, "})(\\s|$)")
  dump_string <- ""
  var_names <- names(list)
  for (v in var_names) {
    vv <- list[[v]]

    if (is.data.frame(vv)) {
      vv <- data.matrix(vv)
    } else if (is.list(vv)) {
      vv <- data_list2array(vv)
    } else if (is.logical(vv)) {
      mode(vv) <- "integer"
    } else if (is.factor(vv)) {
      vv <- as.integer(vv)
    }

    if (!is.numeric(vv))  {
      if (!quiet)
        warning(paste0("variable ", v, " is not supported for dumping."))
      next
    }

    if (!is.integer(vv) && max(abs(vv)) < .Machine$integer.max && (floor(vv) == vv))
      storage.mode(vv) <- "integer"

    if (is.vector(vv)) {
      if (length(vv) == 0) {
        dump_string <- paste(dump_string, paste(v, " <- integer(0)\n"), sep="\n")
        next
      }
      if (length(vv) == 1) {
        dump_string <- paste(dump_string, paste(v, " <- ", as.character(vv)), sep = "\n")
        next
      }
      str <- paste0(v, " <- \nc(", paste(vv, collapse = ', '), ")")
      str <-  gsub(addnlpat, '\\1\n', str)
      dump_string <- paste(dump_string, str, sep="\n")
      l2 <- c(l2, v)
      next
    }

    if (is.matrix(vv) || is.array(vv)) {
      l2 <- c(l2, v)
      vvdim <- dim(vv)
      dump_string <- paste(dump_string, v, " <- ", sep = "\n")
      if (length(vv) == 0) {
        str <- paste0("structure(integer(0), ")
      } else {
        str <- paste0("structure(c(", paste(as.vector(vv), collapse = ', '), "),")
      }
      str <- gsub(addnlpat, '\\1\n', str)
      dump_string <- paste(dump_string,
                           paste(str, ".Dim = c(", paste(vvdim, collapse = ', '), "))\n"),
                           sep = "\n")
      next
    }
  }
  dump_string
}
