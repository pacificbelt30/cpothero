FROM alpine:latest

# docker build -t cpothero:1.0 .
# docker run --rm -it -v /evaldirectory/:/home/cpothero/eval/ 
RUN apk add --no-cache gcc g++ cmake make libc-dev
COPY ./ /home/cpothero