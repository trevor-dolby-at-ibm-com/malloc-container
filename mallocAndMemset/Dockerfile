FROM ubuntu:latest

# docker build -t tdolby/experimental:mallocAndMemset . && docker push tdolby/experimental:mallocAndMemset

ADD mallocAndMemset /usr/local/bin/mallocAndMemset
ADD mmapAnonymousAndMemset /usr/local/bin/mmapAnonymousAndMemset
ADD mmapFileAndMemset /usr/local/bin/mmapFileAndMemset

RUN dd if=/dev/zero of=/tmp/mapfile bs=1M count=512
ENTRYPOINT ["/bin/sleep","3600"]
