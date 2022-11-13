

if [[ -z "${CMD}" ]]; then
	echo env var CMD not set
	exit 1
fi

n=1
while true; echo retry $n : $CMD; do $CMD ; sleep 5; n=n+1; done

