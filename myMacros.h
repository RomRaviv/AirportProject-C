#ifndef MACROS_H_
#define MACROS_H_


#define CHECK_RETURN_0(pointer) (pointer)==(NULL) ? (0):(1)
#define	CHECK_RETURN_NULL(pointer) (pointer)==(NULL) ? (NULL):(pointer)
#define CHECK_MSG_RETURN_0(pointer,msg) {\
	if((pointer)==(NULL)){		\
		puts(msg);				\
		return 0;				\
	}							\
}

#define	CHECK_0_MSG_CLOSE_FILE(val,msg,fp) {\
	if((val)==(0)){				\
		puts(msg);				\
		fclose(fp);				\
		return 0;				\
	}							\
}
#define CHECK_NULL__MSG_CLOSE_FILE(val,msg,fp) {\
	if((val)==(NULL)){			\
		puts(msg);				\
		fclose(fp);				\
		return 0;				\
	}							\
}
#define MSG_CLOSE_RETURN_0(msg,fp) {\
	printf("%s",msg);			\
	fclose(fp);					\
	return 0;					\
}




#endif