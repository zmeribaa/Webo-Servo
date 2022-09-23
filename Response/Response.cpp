/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmeribaa <zmeribaa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/17 14:30:41 by zmeribaa          #+#    #+#             */
/*   Updated: 2022/09/17 15:09:51 by zmeribaa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void) {
	return ;
}

Response::~Response(void) {
	return ;
}

void Response::setHeaders(std::string headers) {
	this->_Headers = headers;
}

void Response::setBody(std::string body) {
	this->_Body = body;	
}

void Response::setStatus(std::string status) {
	this->_Status = status;
}

void Response::setVersion(std::string version) {
	this->_Version = version;
}

void Response::setContentType(std::string contentType) {
	this->_ContentType = contentType;
}

void Response::setContentLength(std::string contentLength) {
	this->_ContentLength = contentLength;
}

void Response::setDate(std::string date) {
	this->_Date = date;
}

void Response::setServer(std::string server) {
	this->_Server = server;
}

std::string Response::getHeaders(void) {
	return this->_Headers;
}

std::string Response::getBody(void) {
	return this->_Body;
}

std::string Response::getStatus(void) {
	return this->_Status;
}

std::string Response::getVersion(void) {
	return this->_Version;
}

std::string Response::getContentType(void) {
	return this->_ContentType;
}

std::string Response::getContentLength(void) {
	return this->_ContentLength;
}

std::string Response::getDate(void) {
	return this->_Date;
}

std::string Response::getServer(void) {
	return this->_Server;
}
